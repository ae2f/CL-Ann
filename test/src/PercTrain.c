#include <ae2fCL/Ann.h>
#include "../test.h"
#include <ae2fCL/Ann/Sp.h>
#include <stdio.h>

#define gLearningRate 0.1
#define gEpochs 10

static ae2f_float_t
Step(ae2f_float_t x) {
    return x >= 0;
}

int main() {
    cl_int err = 0;
    cl_platform_id platform = 0;
    cl_device_id device = 0;
    cl_context context = 0;
    ae2f_err_t err2 = 0;

    err = clGetPlatformIDs(1, &platform, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    context = clCreateContext(0, 1, &device, 0, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    err = ae2fCL_AnnMk(context, 1, &device);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    ae2fCL_AnnSp Perc;
    err2 = ae2fCL_AnnSpMk(
        &Perc, 0, 2,
        Step, 0,
        context, queue, CL_TRUE, 0, 0, 0 
    );
    if(err2) {
        err = err2; goto __failure;
    }

    // [1, 1], [1, 0], [0, 1], [0, 0]
    ae2f_float_t ins[] = {
        1, 1, 1, 0, 0, 1, 0, 0
    };
    // [1, 1], [1, 0], [0, 1], [0, 0]
    cl_mem inbuff = clCreateBuffer(
        context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        sizeof(ins), ins, &err
    );
    if(err) goto __failure;
    ae2f_float_t diff_got[1];
    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = ae2fCL_AnnSpTrain(
            &Perc, inbuff,
            0, 0,
            0, 1, gLearningRate, diff_got, 0,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 1, 1: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnSpTrain(
            &Perc, inbuff,
            0, 2,
            0, 0, gLearningRate, (void*)diff_got, 0,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 1, 0: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnSpTrain(
            &Perc, inbuff,
            0, 4,
            0, 0, gLearningRate, (void*)diff_got, 0,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 0, 1: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnSpTrain(
            &Perc, inbuff,
            0, 6,
            0, 0, gLearningRate, (void*)diff_got, 0,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 0, 0: %f\n\n", diff_got[0]);
    }
    ae2f_float_t outbuff[1] = {  5 };

    err2 = ae2fCL_AnnSpPredictBuffAuto(
        &Perc, ins + 6, outbuff,
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_AnnSpPredictBuffAuto(
        &Perc, ins + 4, outbuff,
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_AnnSpPredictBuffAuto(
        &Perc, ins + 2, outbuff,
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }


    err2 = ae2fCL_AnnSpPredictBuffAuto(
        &Perc, ins, outbuff,
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 1, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    __failure:
    ae2fCL_AnnDel();
    if(context) clReleaseContext(context);
    if(inbuff) clReleaseMemObject(inbuff);
    if(device) clReleaseDevice(device);
    if(queue) clReleaseCommandQueue(queue);
    if(!Perc.mgWeight) ae2fCL_AnnSpDel(&Perc); 
    return err;
}