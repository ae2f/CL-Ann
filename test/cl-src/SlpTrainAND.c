#include "../test.h"
#include <ae2fCL/Ann/Slp.h>
#include <ae2fCL/Ann.h>

#define gLearningRate 0.1
#define gEpochs 1000

static ae2f_float_t
Forward(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t
ForwardPrime(ae2f_float_t output) {
    return output * (1.0 - output);
}

static ae2f_AnnLoss_t Backward;

static ae2f_float_t
Backward(const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
    return (output[i] - target[i]) / c;
}

int main() {
	ae2fCL_AnnSlp* Slp = 0 ;
    cl_int errcl = 0;
    ae2f_err_t err2 = 0, err = 0;
    // [1, 1], [1, 0], [0, 1], [0, 0]
    ae2f_float_t ins[] = {
        1, 1, 1, 0, 0, 1, 0, 0
    };


    const ae2f_float_t goals[] = {
        1, 1, 0, 0
    };

    ae2f_float_t diff_got[2] = {0, 0};

    puts("AnnMkEasy go");

    err = ae2fCL_AnnMkEasy(&errcl);
    puts("AnnMkEasy Check");

    CHECK_ERR(err, CL_SUCCESS, __failure);

    puts("AnnMkEasy done");

    Slp = ae2fCL_AnnSlpMkB(
        2, 0, 0, Forward, ForwardPrime, Backward, 1, 
        0, &err2, &errcl
    );
    ae2f_float_t outbuff[2] = {  5 };


    puts(
		    "Allocation has done.\n" 
		    "Early prediction is starting..."
		    );

    #define ae2fCL_mAnnSlpPredict(obj, inb, _, in_idx, __, out, ...) \
    ae2f_mAnnSlpPredict(&obj->Slp, inb + in_idx, out)

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        0/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f (1, 1)\n", outbuff[0]);

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        6/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f (0, 0)\n", outbuff[0]);

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        4/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f (0, 1)\n", outbuff[0]);

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        2/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f (1, 0)\n", outbuff[0]);

    if(err2) {
        err = err2; goto __failure;
    }
    #define ae2fCL_mAnnSlpTrain(obj, ins, _, in_idx, __, goal, learnrate, ...) ae2f_mAnnSlpTrainB(obj, ins + in_idx, goal, learnrate)

    if(err) goto __failure;

    puts("Training session start");

    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = ae2f_mAnnSlpTrainB(
            &Slp->Slp, ins,
            goals + 0, gLearningRate
        );
        if(err2) {
            err = err2; 
            puts("TRAIN DOOMED");
            printf("%d\n", err2);
            goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            &Slp->Slp, ins + 2/*in_idx*/,
            goals + 2, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            &Slp->Slp, ins + 4/*in_idx*/,
            goals + 2, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            &Slp->Slp, ins + 6/*in_idx*/,
            goals + 2, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }
    }

    puts("we are going for predict");

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        0/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 1, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        6/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        err2 = ae2f_mAnnSpPredict(ae2f_mAnnSlpPerV(&Slp->Slp, 0), ins + 6, outbuff);
        printf("Checking the value (SP): %f\n", outbuff[0]);

        printf("AND 0, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        4/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        2/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    __failure:
    puts("Deinit");
    if(Slp) ae2f_AnnSlpDel(Slp);
    printf("ERR: %d\n", err);
    printf("ERR-CL: %d\n", ae2fCL_Ann.LErr);
    ae2fCL_AnnDel();
    return err;
}
