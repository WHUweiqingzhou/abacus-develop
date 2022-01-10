//==========================================================
// Used to genarate ucell
//==========================================================

#include "../../input.h"
#include "../../module_cell/unitcell_pseudo.h"
#include "../../module_neighbor/sltk_atom_arrange.h"
#include "../../module_neighbor/sltk_grid_driver.h"

void setupcell(UnitCell_pseudo &ucell)
{
    ucell.ntype = 1;

	ucell.atoms = new Atom[ucell.ntype];
	ucell.set_atom_flag = true;

    delete[] ucell.atom_label;
    delete[] ucell.atom_mass;
    ucell.atom_mass  = new double[ucell.ntype];
	ucell.atom_label = new std::string[ucell.ntype];
    ucell.atom_mass[0] = 39.948;
    ucell.atom_label[0] = "Ar";

    ucell.lat0 = 1;
    ucell.lat0_angstrom = ucell.lat0 * 0.529177;
    ucell.tpiba  = ModuleBase::TWO_PI / ucell.lat0;
	ucell.tpiba2 = ucell.tpiba * ucell.tpiba;

    ucell.latvec.e11 = ucell.latvec.e22 = ucell.latvec.e33 = 10;
    ucell.latvec.e12 = ucell.latvec.e13 = ucell.latvec.e23 = 0;
    ucell.latvec.e21 = ucell.latvec.e31 = ucell.latvec.e32 = 0;

    ucell.a1.x = ucell.latvec.e11;
	ucell.a1.y = ucell.latvec.e12;
	ucell.a1.z = ucell.latvec.e13;

	ucell.a2.x = ucell.latvec.e21;
	ucell.a2.y = ucell.latvec.e22;
	ucell.a2.z = ucell.latvec.e23;

	ucell.a3.x = ucell.latvec.e31;
	ucell.a3.y = ucell.latvec.e32;
	ucell.a3.z = ucell.latvec.e33;

    ucell.nat = 4;
    ucell.atoms[0].na = 4;
    ucell.set_vel = 1;

    delete[] ucell.atoms[0].tau;
	delete[] ucell.atoms[0].taud;
	delete[] ucell.atoms[0].vel;
    delete[] ucell.atoms[0].mbl;
    ucell.atoms[0].tau = new ModuleBase::Vector3<double>[4];
    ucell.atoms[0].taud = new ModuleBase::Vector3<double>[4];
	ucell.atoms[0].vel = new ModuleBase::Vector3<double>[4];
    ucell.atoms[0].mbl = new ModuleBase::Vector3<int>[4];
    ucell.atoms[0].mass = ucell.atom_mass[0];

    ucell.atoms[0].taud[0].set(0.0, 0.0, 0.0);
    ucell.atoms[0].taud[1].set(0.52, 0.52, 0.0);
    ucell.atoms[0].taud[2].set(0.51, 0.0, 0.5);
    ucell.atoms[0].taud[3].set(0.0, 0.53, 0.5);
    ucell.atoms[0].vel[0].set(-0.000132080736364, 7.13429429835e-05, -1.40179977966e-05);
    ucell.atoms[0].vel[1].set(0.000153039878532, -0.000146533266608, 9.64491480698e-05);
    ucell.atoms[0].vel[2].set(-0.000133789480226, -3.0451038112e-06, -5.40998380137e-05);
    ucell.atoms[0].vel[3].set(0.000112830338059, 7.82354274358e-05, -2.83313122596e-05);
    for(int ia=0; ia<4; ++ia)
    {
        ucell.atoms[0].tau[ia] = ucell.atoms[0].taud[ia] * ucell.latvec;
        ucell.atoms[0].mbl[ia].set(1, 1, 1);
    }

    ucell.omega = abs( ucell.latvec.Det() ) * ucell.lat0 * ucell.lat0 * ucell.lat0;

    ucell.GT = ucell.latvec.Inverse();
	ucell.G  = ucell.GT.Transpose();
	ucell.GGT = ucell.G * ucell.GT;
	ucell.invGGT = ucell.GGT.Inverse();

    ucell.GT0 = ucell.latvec.Inverse();
    ucell.G0  = ucell.GT.Transpose();
    ucell.GGT0 = ucell.G * ucell.GT;
    ucell.invGGT0 = ucell.GGT.Inverse();

	ucell.set_iat2itia();
}

void neighbor(Grid_Driver &grid_neigh, UnitCell_pseudo &ucell)
{
    ofstream ofs("run.log");
    GlobalV::SEARCH_RADIUS = 8.5 * ModuleBase::ANGSTROM_AU;
    INPUT.mdp.rcut_lj *= ModuleBase::ANGSTROM_AU;
	INPUT.mdp.epsilon_lj /= ModuleBase::Hartree_to_eV;
	INPUT.mdp.sigma_lj *= ModuleBase::ANGSTROM_AU;

    atom_arrange::search(1, ofs, grid_neigh, ucell, GlobalV::SEARCH_RADIUS, 0, 0);
}