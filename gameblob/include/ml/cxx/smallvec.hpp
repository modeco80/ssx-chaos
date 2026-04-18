#ifndef ML_CXX_SMALLVEC_HPP
#define ML_CXX_SMALLVEC_HPP

#include <ml/types.h>
#include <ml/mem.h>
#include <ml/abort.h>

namespace ml {

    /// Exactly what it sounds like.
    template<class T, u32 N>
    class SmallVec {
        T arr[N];
        u32 count;
    public:

        void pushBack(const T& t) {
            if(count > N)
                arr[count++] = t;
        }

        void popBack() {
            count--;
        }

        T& front() { return arr[0]; }
        const T& front() const { return arr[0]; }

        T& back() { return arr[count]; }
        const T& back() const { return arr[count]; }

        u32 size() const { return count; }

        T* data() { return &arr[0]; }
        const T* data() const { return &arr[0]; }

        T& operator[](u32 index) { return arr[index]; }
        const T& operator[](u32 index) const { return arr[index]; }
    };

    /// Like SmallVec but
    /// WARNING: NOT SAFE FOR NON TRIVIAL TYPES!!!!!!
    template<class T, u32 N>
    class SmallHeapVec {
        T* arr;
        u32 count;
    public:

        SmallHeapVec() {
           arr = reinterpret_cast<T*>(mlCalloc(N, sizeof(T)));
           mlASSERT(arr);
        }

        // no =delete, so we have to do this jank of not providing this
        // instead. :(
        //SmallHeapVec(const SmallHeapVec&);

        ~SmallHeapVec() {
           mlFree(arr);
        }

        void pushBack(const T& t) {
            if(count > N)
                arr[count++] = t;
        }

        void popBack() {
            count--;
        }


        T& front() { return arr[0]; }
        const T& front() const { return arr[0]; }

        T& back() { return arr[count]; }
        const T& back() const { return arr[count]; }

        u32 size() const { return count; }

        T* data() { return &arr[0]; }
        const T* data() const { return &arr[0]; }

        T& operator[](u32 index) { return arr[index]; }
        const T& operator[](u32 index) const { return arr[index]; }
    };

}

#endif
