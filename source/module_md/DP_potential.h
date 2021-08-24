#ifndef DP_POTENTIAL_H
#define DP_POTENTIAL_H

#include "../module_base/vector3.h"
#include "../module_base/matrix.h"
#include "../module_cell/unitcell_pseudo.h"

class DP_potential
{
public:

    DP_potential();
    ~DP_potential();

    static void DP_init(UnitCell_pseudo &ucell_c, 
                std::vector<double> &cell, 
                std::vector<double> &coord, 
                std::vector<int> &atype);

    static void DP_pot(UnitCell_pseudo &ucell_c, 
                double &potential, 
                Vector3<double> *force, 
                ModuleBase::matrix &stress);
    
};

#endif