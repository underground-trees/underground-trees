// Dynamic arrays: An interface that makes sense to me, with an STL implementation.
// Avner, 4.7.24, from the earlier implementation during 20-21.

#include <vector>
#include <algorithm>

template<typename T>
class DynArrSTLImp
{
    private:
    protected:
        std::vector<T> VecDM;

    public:
        unsigned int Size (void) const
            { return (VecDM.size()); }
        bool IsEmpty (void) const
            { return (Size() == 0); }
        const T& Last (unsigned back=0) const
            { return (*(VecDM.end() - (back+1))); }
        T& Last (unsigned back=0) 
            { return (*(VecDM.end() - (back+1))); }
        void Add (const T& val=T())
            { VecDM.push_back(val); }
        void Insert (unsigned index, const T& val=T())
            { VecDM.insert (VecDM.begin()+index, val); }
        void Remove (unsigned index, bool MaintainOrder=true)
            {
                if (MaintainOrder)
                    VecDM.erase (std::remove (VecDM.begin()+index, VecDM.begin()+index+1, VecDM[index]),
                                 VecDM.end());
                else {
                    VecDM[index] = Last();
                    VecDM.resize (Size()-1);
                }
            }
        void RemoveLast (unsigned amount=1)
            {
                VecDM.erase (VecDM.end()-amount, VecDM.end());
            }
        void Empty (void)
            { VecDM.clear(); }
        T& operator[] (unsigned index)
            { return (VecDM[index]); }
        const T& operator[] (unsigned index) const
            { return (VecDM[index]); }

};
