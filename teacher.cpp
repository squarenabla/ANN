#include "teacher.h"

int fann_set_train_data(struct fann_train_data* data,
                        unsigned int num, fann_type* input,
                        fann_type* output) {
    if(num >= data->num_data) {
        fann_error(NULL, FANN_E_INDEX_OUT_OF_BOUND);
        return -1;
    }

    for (unsigned int i = 0; i < data->num_input; i++) {
        data->input[num][i] = input[i];
    }

    for (unsigned int i=0; i<data->num_output ;i++) {
        data->output[num][i] = output[i];
    }

    return 0;
}

template <class InputIterator, class OutputIterator, class UnaryOperator, class Param>
OutputIterator transform (InputIterator first1, InputIterator last1,
                          OutputIterator result, UnaryOperator op, Param param1) {
    while (first1 != last1) {
        *result = op(*first1, param1);  // or: *result=binary_op(*first1,*first2++);
        ++result;
        ++first1;
    }
    return result;
}

Teacher::Teacher(QObject *parent) : QObject(parent) {
    //default values
    _desiredError = (qreal) 0.00001;
    _maxEpochs = 50000;
    _epochsBetweenReports = 100;

    _numLayers = 5;
    _numNeuronsInLayer.push_back(ELECTRODENUM);
    for (quint16 i = 0; i < 3; i++) {
        _numNeuronsInLayer.push_back(2);
    }
    _numNeuronsInLayer.push_back(1);
}

void Teacher::setLayers(const ANNLayers hiddenLayers) {
    _numLayers = hiddenLayers.size() + 2;
    _numNeuronsInLayer = hiddenLayers;
    _numNeuronsInLayer.push_front(ELECTRODENUM);
    _numNeuronsInLayer.push_back(1);

    qDebug() << "Neurons";
    printVector(_numNeuronsInLayer);
    /*
    for(quint16 i = 0; i < hiddenLayers; i++){
            _numNeuronsInLayer.push_back(layers[i]);
    }*/
}

void Teacher::createTrainData(const QVector<EMGFourierData> data) {
    qDebug()<<"createTrainData";
    _data = fann_create_train( data.size(), BUFFER_SIZE * ELECTRODENUM, 1 );

    fann_type input[BUFFER_SIZE * ELECTRODENUM];
    fann_type output[1];

    qDebug() << data.size() << " " << data[0].fourierArray.size() << data[0].fourierArray[0].size() << "==?"<<BUFFER_SIZE;

    for (int i = 0; i < data.size(); i++) {
        for (unsigned int j = 0; j < ELECTRODENUM; j++) {
            for (unsigned int k = 0; k < data[i].fourierArray[j].size(); k++) {
                input[j * BUFFER_SIZE + k] = (float)data[i].fourierArray[j][k];
            }
            //input[j] = ((float)data[i].electrodeEMG[j]);
        }
        output[0] = ((float)data[i].movementIndex)/MAXAMLVALUE;

        fann_set_train_data(_data, i, input, output);
    }
}

void Teacher::trainOnFile(const char *fileName) {
    qDebug()<<"trainOnFile";

    struct fann *ann = fann_create_standard_array(_numLayers, (unsigned int*)_numNeuronsInLayer.constData());

    fann_set_train_error_function(ann, FANN_ERRORFUNC_TANH);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_file(ann, fileName, _maxEpochs, _epochsBetweenReports, _desiredError);
    fann_save(ann, FANNFILENAME);
    fann_destroy(ann);
}

void Teacher::trainOnData() {
    qDebug()<<"trainOnData";
    qDebug()<<"params";
    //const short unsigned int* data = (_numNeuronsInLayer.constData());
    //unsigned int* d = (unsigned int *)data;
    _numLayers = 3;
    unsigned int data[_numLayers];
    data[0] = BUFFER_SIZE * ELECTRODENUM;
    data[1] = (unsigned int)(2 * sqrt((double)BUFFER_SIZE * ELECTRODENUM));
    data[2] = 1;
//    for (unsigned int i = 0; i < _numLayers; i++) {
//        data[i] = (unsigned int)_numNeuronsInLayer[i];
//    }
    struct fann *ann = fann_create_standard_array(_numLayers, data);


    fann_print_parameters(ann);

    fann_set_train_error_function(ann, FANN_ERRORFUNC_TANH);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    //fann_set_scaling_params(ann, _data, -1.0, 1.0, -1.0, 1.0);
    qDebug()<<"ann created";

    fann_save_train(_data, "./data/beforescaling");

    //fann_scale_train(ann, _data);
    //qDebug()<<"data scaled";
    //fann_save_train(_data, "./data/afterscaling");


    fann_train_on_data(ann, _data, _maxEpochs, _epochsBetweenReports, _desiredError);
    qDebug()<<"fann trained";
    fann_save(ann, FANNFILENAME);
    qDebug()<<"fann saved";
    fann_destroy(ann);
}

