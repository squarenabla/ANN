#include "executor.h"

Executor::Executor(QObject *parent) : QObject(parent) {}

void Executor::SetANN(const char *fileName) {
    _ann = fann_create_from_file(fileName);
}

void Executor::Execute(const EMGRMSData &data, QVector<float> &output) {
    std::vector<float> input;
    for (int i = 0; i < data.rmsArray.size(); i++) {
        //for (int j = 0; j < data.waveletArray[i].size(); j++) {
            input.push_back((float)data.rmsArray[i]);
        //}
    }

    fann_type *result = (fann_run(_ann, input.data()));
    std::vector<float> stdOutput(result, result + MOVE_NUM);
    output = QVector<float>::fromStdVector(stdOutput);
}
