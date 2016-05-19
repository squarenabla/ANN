#ifndef RMS
#define RMS

#include <QVector>
#include <algorithm>

template <typename T>
T RootMeanSquare(const QVector<T> &data) {
    T result = 0;
    for (int i = 0; i < data.size(); i++) {
        result += data[i] * data[i];
    }
    result = (T)sqrt((double)result);
    return result;
}

template <typename T>
void WaveletRMS(const QVector<T> &data, QVector<T> &result) {
    result.clear();
    int index = 1;
    for (int i = 1; i < data.size(); i *= 2) {
        QVector<T> buffer = data.mid(index, i);
        result.push_back(RootMeanSquare(buffer));
        index += i;
    }
}

#endif // RMS

