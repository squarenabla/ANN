#ifndef WAVELET
#define WAVELET

#include <QVector>

template <typename T>
void WaveletHaar(const QVector<T> signal, QVector<T> &result) {
    result = signal;
    QVector<T> buffer(signal.size());

    for (int halfSize = signal.size()/2; halfSize > 0; halfSize /= 2) {
        for (int i = 0; i < halfSize; i++) {
            buffer[i] = (result[2 * i] + result[2 * i + 1])/2.0;
            buffer[halfSize + i] = (result[2 * i] - result[2 * i + 1])/2.0;
        }

        for (int i = 0; i < halfSize; i++) {
            result[i] = buffer[i];
        }
    }

}

#endif // WAVELET
