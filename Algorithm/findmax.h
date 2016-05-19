template <class ForwardIterator>
unsigned int MaxElement (ForwardIterator first,ForwardIterator last) {
    unsigned int largestIndex = 0;
    unsigned int currentIndex = 0;
    if (first == last)
        return largestIndex;

    ForwardIterator largest = first;

    while (++first != last) {
        ++currentIndex;
        if (*largest < *first) {
            largest = first;
            largestIndex = currentIndex;
        }
    }
    return largestIndex;
}
