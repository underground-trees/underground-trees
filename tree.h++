/******************************************************************************/
/*                                                                            */
/*                      U N D E R G R O U N D   T R E E S                     */
/*                                                                            */
/*                                                                            */
/* Trees, operating on the type of the inherited class, not the type from the */
/* Trees package! Lends itself to inheritance of more specific trees.         */
/* With iterators and useful access functions.                                */
/*                                                                            */
/* Version 1.1                                                                */
/*                                                                            */
/* Avner 11-12.24, from the implementations around 20-21 and earlier,         */
/* back to 2001.                                                              */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/******************************************************************************/



namespace ProdCAD
{

namespace UnderGround
{

template<typename TREET, template<typename ARRITEMT> class ARRT>
class Tree
{
    protected:
        Tree (void) :
            ParentDM(NULL)
            { ; }
        Tree (TREET* parent) :
            ParentDM(parent)
            { ; }
        ~Tree (void)
            { RemoveChildren(); }

        void SetParent (TREET* parent)
            { ParentDM = parent; }

    public:
        void RemoveChildren (bool del=true)
            {
                if (del)
                    for (unsigned i=0; i<Children(); i++)
                        if (Child(i))
                            delete Child(i);
                ChildArrDM.Empty();
            }
        TREET& AddNewChild (void)
            {
                ChildArrDM.Add (new TREET());
                LastChild()->SetParent (static_cast<TREET*>(this));
                return (*LastChild());
            }
        TREET& InsertNewChild (unsigned index)
            {
                ChildArrDM.Insert (index, new TREET());
                ChildArrDM[index]->SetParent (static_cast<TREET*>(this));
                return (*Child(index));
            }
        void AddChild (TREET* child)
            {
                ChildArrDM.Add (child);
                if (child)
                    child->SetParent (static_cast<TREET*>(this));
            }
        void InsertChild (unsigned index, TREET* child)
            {
                ChildArrDM.Insert (index, child);
                if (child)
                    child->SetParent (static_cast<TREET*>(this));
            }
        void RemoveChild (unsigned index, bool MaintainOrder=true, bool del=true)
            {
                if (del)
                    if (Child(index))
                        delete Child(index);
                ChildArrDM.Remove (index, MaintainOrder);
            }
        bool RemoveChild (const TREET* child, bool MaintainOrder=true, bool del=true)
            {
                for (unsigned i=0; i<Children(); i++) {
                    if (Child(i) == child) {
                        RemoveChild (i, MaintainOrder, del);
                        return (true);
                    }
                }
                return (false);
            }

        const TREET* Parent (void) const
            { return (ParentDM); }
        TREET* Parent (void)
            { return (ParentDM); }
        unsigned Children (void) const
            { return (ChildArrDM.Size()); }
        const TREET* Child (unsigned index) const
            { return (ChildArrDM[index]); }
        TREET* Child (unsigned index)
            { return (ChildArrDM[index]); }
        const TREET* LastChild (void) const
            { return (Child (Children()-1)); }
        TREET* LastChild (void)
            { return (Child (Children()-1)); }
        const TREET& Root (void) const
            {
                const TREET* curr = static_cast<const TREET*>(this);
                while (curr->Parent())
                    curr = curr->Parent();
                return (*curr);
            }
        TREET& Root (void)
            {
                TREET* curr = static_cast<TREET*>(this);
                while (curr->Parent())
                    curr = curr->Parent();
                return (*curr);
            }
        bool IsLeaf (void) const
            { return (Children() == 0); }
        bool IsRoot (void) const
            { return (ParentDM == NULL); }
        unsigned Level (void) const
            {
                unsigned level = 1;
                const TREET* curr = static_cast<const TREET*>(this);
                while (curr = curr->Parent())
                    level++;
                return (level);
            }

        // For these Find function, the tree should implement a Val() function.
        // Every Val() is compared to the given val using the chosen iterator.
        template<typename VALT>
        bool Find (VALT val, bool DFS, const TREET*& res) const;
        template<typename VALT>
        bool Find (VALT val, bool DFS, TREET*& res);


    private:
    protected:
        ARRT <TREET*>ChildArrDM;
        TREET* ParentDM;

};

template<typename TREET, template<typename ARRITEMT> class ARRT>
class DFSIter
{
    public:
        DFSIter (TREET& tree) :
            RootDM(tree), CurrentDM(&tree), CurrParentDM(&tree)
            { ; }
        virtual ~DFSIter()
            { ; }

        void Init (void)
            {
                CurrentDM = CurrParentDM = &RootDM;
                IndexArrDM.Empty();
                IndexArrDM.Add (0);
            }
        bool Done (void) const
            { return (IndexArrDM.IsEmpty()); }
        virtual void AscendingCurrent (void)
            { ; }
        virtual void CurrentVisited (void)
            { ; }
        virtual void Descending (void)
            { ; }
        void Advance (void)
            {
                while (CurrentDM == NULL ||
                    IndexArrDM.Last() >= CurrentDM->Children()) {
                    AscendingCurrent();
                    IndexArrDM.RemoveLast();
                    if (IndexArrDM.IsEmpty()) {
                        // verify (CurrentDM == &RootDM);
                        return;
                    }

                    CurrentDM = CurrParentDM;
                    CurrParentDM = CurrParentDM->Parent();
                    IndexArrDM.Last()++;
                    CurrentVisited();
                }

                Descending();
                CurrParentDM = CurrentDM;
                CurrentDM = CurrentDM->Child (IndexArrDM.Last());
                IndexArrDM.Add (0);
            }
        unsigned Level (void) const
            { return (IndexArrDM.Size()); }
        operator TREET* (void) const
            { return (CurrentDM); }
        TREET* operator-> (void) const
            { return (CurrentDM); }

    private:
    protected:
        TREET& RootDM;
        TREET* CurrentDM;
        TREET* CurrParentDM;
        ARRT<unsigned> IndexArrDM;
};

#define TreeDFSIterLoop(iter) for (iter.Init(); !iter.Done(); iter.Advance())


template<typename TREET, template<typename ARRITEMT> class ARRT>
class BFSIter
{
    private:
    protected:
        unsigned IndexDM;
        ARRT<TREET*> PendingDM;

    public:
        BFSIter (TREET& root) :
            IndexDM(0)
            {
                PendingDM.Add (&root);
            }
        ~BFSIter()
            { ; }

        void Init (void)
            {
                if (PendingDM.Size())
                    PendingDM.RemoveLast (PendingDM.Size()-1);
                IndexDM = 0;
            }
        bool Done (void) const
            { return (IndexDM >= PendingDM.Size()); }
        void Advance (void)
            {
                if (PendingDM[IndexDM])
                    for (unsigned i=0; i<PendingDM[IndexDM]->Children(); i++)
                        PendingDM.Add (PendingDM[IndexDM]->Child(i));
                IndexDM++;
            }

        operator TREET* (void) const
            { return (PendingDM[IndexDM]); }
        TREET* operator-> (void) const
            { return (PendingDM[IndexDM]); }
};

#define TreeBFSIterLoop(iter) for (iter.Init(); !iter.Done(); iter.Advance())



template<typename TREET, template<typename ARRITEMT> class ARRT>
template<typename VALT>
bool Tree<TREET, ARRT>::Find (VALT val, bool DFS, const TREET*& res) const
{
    if (DFS) {
        DFSIter<const TREET, ARRT> iter (*(static_cast<const TREET*>(this)));
        TreeDFSIterLoop (iter) {
            // if (iter && *iter==val) {
            if (iter && iter->Val()==val) {
                res = iter;
                return (true);
            }
        }
    }
    else {
        BFSIter<const TREET, ARRT> iter (*(static_cast<const TREET*>(this)));
        TreeBFSIterLoop (iter) {
            // if (iter && *iter==val) {
            if (iter && iter->Val()==val) {
                res = iter;
                return (true);
            }
        }
    }
    return (false);
}

template<typename TREET, template<typename ARRITEMT> class ARRT>
template<typename VALT>
bool Tree<TREET, ARRT>::Find (VALT val, bool DFS, TREET*& res)
{
    if (DFS) {
        DFSIter<TREET, ARRT> iter (*(static_cast<TREET*>(this)));
        TreeDFSIterLoop (iter) {
            // if (iter && *iter==val) {
            if (iter && iter->Val()==val) {
                res = iter;
                return (true);
            }
        }
    }
    else {
        BFSIter<TREET, ARRT> iter (*(static_cast<TREET*>(this)));
        TreeBFSIterLoop (iter) {
            // if (iter && *iter==val) {
            if (iter && iter->Val()==val) {
                res = iter;
                return (true);
            }
        }
    }
    return (false);
}

};

};








