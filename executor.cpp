#include "executor.h"

Executor::Executor(QObject *parent) : QObject(parent){}

void Executor::setANN(const char *fileName){
    _ann = fann_create_from_file(fileName);
}

void Executor::execute(QVector<float> input, QVector<float> &output){
    fann_type *result = (fann_run(_ann, input.data()));
    std::vector<float> stdOutput(result, result + sizeof(result)/sizeof(fann_type));
    output = QVector<float>::fromStdVector(stdOutput);
}
