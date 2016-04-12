#include "executor.h"

Executor::Executor(QObject *parent) : QObject(parent) {}

void Executor::SetANN(const char *fileName) {
    _ann = fann_create_from_file(fileName);
}

void Executor::Execute(const EMGFourierData &data, QVector<float> &output) {
    std::vector<float> input;
    for (int i = 0; i < data.fourierArray.size(); i++) {
        for (int j = 0; j < data.fourierArray[i].size(); j++) {
            input.push_back((float)data.fourierArray[i][j]);
        }
    }

    fann_type *result = (fann_run(_ann, input.data()));
    std::vector<float> stdOutput(result, result + sizeof(result)/sizeof(fann_type));
    output = QVector<float>::fromStdVector(stdOutput);
}
