/******************************************************************************/
/*                                                                            */
/*                      U N D E R G R O U N D   T R E E S                     */
/*                                                                            */
/* Tests for the underground trees, inheritance, iterators, and other basic   */
/* functionality.                                                             */
/*                                                                            */
/* Avner 11-12.24                                                             */
/*                                                                            */
/* This distribution of underground trees is open source (aka free) software. */
/* It is distributed under conditions specified in a license (comprises of    */
/* license.txt, and the accompanying docs LGPL.txt, GPL.txt).                 */
/* See a formal notice about this distribution being open source in section 3 */
/* of license.txt .                                                           */
/* The distribution is provided without claiming anything, and offers no      */
/* guarantee of any kind, no warranty, and no liability, whatsoever.          */
/*                                                                            */
/******************************************************************************/



#include <iostream>
#include "DynArrSTLImp.h++"
#include "tree.h++"


using namespace ProdCAD;
using namespace ProdCAD::UnderGround;


#define DynArray DynArrSTLImp


class DerInstTree : public Tree<DerInstTree, DynArray>
{
    public:
        DerInstTree (unsigned val=0) :
            ValDM(val)
            { ; }

        unsigned Val (void) const
            { return (ValDM); }

        void SetVal (unsigned val)
            { ValDM = val; }

        DerInstTree& AddNewChild (unsigned val)
            {
                ChildArrDM.Add (new DerInstTree (val));
                LastChild()->SetParent (this);
                return (*LastChild());
            }

    private:
        unsigned ValDM;
};


void Test1Gen()
{
    DerInstTree bt;
    std::cout << "Val: " << bt.Val() << ", children: " << bt.Children() << std::endl;
    DerInstTree& btc1 = bt.Tree::AddNewChild();
    bt.LastChild()->SetVal (5);
    DerInstTree& btc2 = bt.AddNewChild (4);
    btc1.AddNewChild (1);
    btc1.Tree::AddChild (NULL);
    btc1.AddNewChild (2);
    btc1.AddNewChild (3);

    DFSIter<DerInstTree, DynArray> diter (bt);
    TreeDFSIterLoop (diter)
        if ((const DerInstTree*)diter)
            std::cout << std::string(2*diter.Level(),' ') << "Val: " << diter->Val() << ", children: " << diter->Children() <<
                ", level: " << diter->Level() <<
                (diter->IsLeaf() ? " Leaf. " : "") << (diter->IsRoot() ? " Root. " : "") << std::endl;
        else
            std::cout << std::string(2*diter.Level(),' ') << "Null. " << std::endl;

    DFSIter<const DerInstTree, DynArray> dciter (bt);
    TreeDFSIterLoop (dciter)
        if ((const DerInstTree*)dciter)
            std::cout << std::string(2*dciter.Level(),' ') << "Val: " << dciter->Val() << ", children: " << dciter->Children() <<
                ", level: " << dciter->Level() <<
                (dciter->IsLeaf() ? " Leaf. " : "") << (dciter->IsRoot() ? " Root. " : "") << std::endl;
        else
            std::cout << std::string(2*dciter.Level(),' ') << "Null. " << std::endl;

    BFSIter<DerInstTree, DynArray> biter (bt);

    TreeBFSIterLoop (biter)
        if ((const DerInstTree*)biter)
            std::cout << std::string(2*biter->Level(),' ') << "Val: " << biter->Val() << ", children: " << biter->Children() <<
                ", level: " << biter->Level() <<
                (biter->IsLeaf() ? " Leaf. " : "") << (biter->IsRoot() ? " Root. " : "") << std::endl;
        else
            std::cout << "Null. " << std::endl;

    DerInstTree* t4;
    if (bt.Find (4, true, t4))
        std::cout << "val of t4: " << t4->Val() << std::endl;
    if (bt.Find (4, false, t4))
        std::cout << "val of t4 (BFS): " << t4->Val() << std::endl;
    DerInstTree* t5;
    if (bt.Find (5, true, t5))
        std::cout << "val of t5 (BFS): " << t5->Val() << std::endl;
    if (!bt.Find (6, false, t4))
        std::cout << "val of 6 wasn't found. " << std::endl;
}

void Test3Removal()
{
    DerInstTree bt1 (12);
    bt1.InsertNewChild (0);
    DerInstTree bt2 (42);
    bt1.InsertChild (0,&bt2);
    DFSIter<DerInstTree, DynArrSTLImp> diter (bt1);
    TreeDFSIterLoop (diter)
        std::cout << std::string(2*diter.Level(),' ') << "Val: " << diter->Val() <<
                     ", children: " << diter->Children() << std::endl;

    const DerInstTree* cres;
    std::cout << "4267 " <<
                (bt1.Find (4267, true, cres) ? "" : "Not ") << "found!" << std::endl;
    DerInstTree* res;
    std::cout << "42 " <<
                (bt1.Find (42, false, res) ? "" : "Not ") << "found!" << std::endl;
    bt1.RemoveChild (res, false, false);
    TreeDFSIterLoop (diter)
            std::cout << std::string(2*diter.Level(),' ') << "Val: " << diter->Val() <<
                ", children: " << diter->Children() << std::endl;
}

int main()
{
    Test1Gen();
    Test3Removal();
    return (0);
}




