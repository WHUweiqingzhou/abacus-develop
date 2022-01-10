#include "../MD_func.h"
#include "../LJ_potential.h"
#include "setcell.cpp"
#include "gtest/gtest.h"

UnitCell_pseudo ucell;
class MD_func_test : public testing::Test
{
protected:
	double *allmass;                     // atom mass 
    ModuleBase::Vector3<double> *pos;    // atom position
    ModuleBase::Vector3<double> *vel;    // atom velocity
    ModuleBase::Vector3<int> *ionmbl;    // atom is frozen or not
    ModuleBase::matrix virial;           // virial for this lattice
	ModuleBase::matrix stress;           // stress for this lattice
    double potential;                    // potential energy
	int natom;                           // atom number
	double temperature;                  // temperature
	int frozen_freedom;                  // frozen_freedom

	static void SetUpTestCase()
	{
		setupcell(ucell);
	}

	static void TearDownTestCase(){}
};

TEST_F(MD_func_test, gaussrand)
{
	EXPECT_DOUBLE_EQ(MD_func::gaussrand(), -0.36873170048963488);
    EXPECT_DOUBLE_EQ(MD_func::gaussrand(), -0.52126317699631741);
    EXPECT_DOUBLE_EQ(MD_func::gaussrand(), 0.58546002948078557);
}

TEST_F(MD_func_test, initpos)
{
	natom = ucell.nat;
	pos = new ModuleBase::Vector3<double> [natom];
	MD_func::InitPos(ucell, pos);

	EXPECT_DOUBLE_EQ(pos[0].x, 0.0);
    EXPECT_DOUBLE_EQ(pos[0].y, 0.0);
	EXPECT_DOUBLE_EQ(pos[0].z, 0.0);
	EXPECT_DOUBLE_EQ(pos[1].x, 5.2);
    EXPECT_DOUBLE_EQ(pos[1].y, 5.2);
	EXPECT_DOUBLE_EQ(pos[1].z, 0.0);
	EXPECT_DOUBLE_EQ(pos[2].x, 5.1);
    EXPECT_DOUBLE_EQ(pos[2].y, 0.0);
	EXPECT_DOUBLE_EQ(pos[2].z, 5.0);
	EXPECT_DOUBLE_EQ(pos[3].x, 0.0);
    EXPECT_DOUBLE_EQ(pos[3].y, 5.3);
	EXPECT_DOUBLE_EQ(pos[3].z, 5.0);
}

TEST_F(MD_func_test, randomvel)
{
	ucell.set_vel = 0;
	temperature = 300 / ModuleBase::Hartree_to_K;
	allmass = new double [natom];
	ionmbl = new ModuleBase::Vector3<int> [natom];
	vel = new ModuleBase::Vector3<double> [natom];
	MD_func::InitVel(ucell, temperature, allmass, frozen_freedom, ionmbl, vel);

	EXPECT_DOUBLE_EQ(vel[0].x, 6.0690078968654719e-05);
    EXPECT_DOUBLE_EQ(vel[0].y, -0.00014603859011949659);
	EXPECT_DOUBLE_EQ(vel[0].z, 3.5215704435916494e-05);
	EXPECT_DOUBLE_EQ(vel[1].x, 5.5672261036849076e-05);
    EXPECT_DOUBLE_EQ(vel[1].y, 0.00016510552718707551);
	EXPECT_DOUBLE_EQ(vel[1].z, -3.853681367488217e-05);
	EXPECT_DOUBLE_EQ(vel[2].x, 1.2878622979722992e-05);
    EXPECT_DOUBLE_EQ(vel[2].y, -8.79320431251751e-05);
	EXPECT_DOUBLE_EQ(vel[2].z, -0.00012075887140803323);
	EXPECT_DOUBLE_EQ(vel[3].x, -0.00012924096298522683);
    EXPECT_DOUBLE_EQ(vel[3].y, 6.8865106057596176e-05);
	EXPECT_DOUBLE_EQ(vel[3].z, 0.00012407998064699891);
}

TEST_F(MD_func_test, getmassmbl)
{
	for(int i=0; i<natom; ++i)
	{
		EXPECT_DOUBLE_EQ(allmass[i], 39.948 / ModuleBase::AU_to_MASS);
		EXPECT_TRUE(ionmbl[i].x == 1);
		EXPECT_TRUE(ionmbl[i].y == 1);
		EXPECT_TRUE(ionmbl[i].z == 1);
	}

	EXPECT_TRUE(frozen_freedom == 3);
}

TEST_F(MD_func_test, readvel)
{
	MD_func::ReadVel(ucell, vel);

	EXPECT_DOUBLE_EQ(vel[0].x, -0.0001320807363640);
    EXPECT_DOUBLE_EQ(vel[0].y, 7.13429429835e-05);
	EXPECT_DOUBLE_EQ(vel[0].z, -1.40179977966e-05);
	EXPECT_DOUBLE_EQ(vel[1].x, 0.000153039878532);
    EXPECT_DOUBLE_EQ(vel[1].y, -0.000146533266608);
	EXPECT_DOUBLE_EQ(vel[1].z, 9.64491480698e-05);
	EXPECT_DOUBLE_EQ(vel[2].x, -0.000133789480226);
    EXPECT_DOUBLE_EQ(vel[2].y, -3.0451038112e-06);
	EXPECT_DOUBLE_EQ(vel[2].z, -5.40998380137e-05);
	EXPECT_DOUBLE_EQ(vel[3].x, 0.000112830338059);
    EXPECT_DOUBLE_EQ(vel[3].y, 7.82354274358e-05);
	EXPECT_DOUBLE_EQ(vel[3].z, -2.83313122596e-05);
}

TEST_F(MD_func_test, kinetic_stress)
{
	double kinetic;
	stress.create(3,3);
	MD_func::kinetic_stress(ucell, vel, allmass, kinetic, stress);
	EXPECT_DOUBLE_EQ(kinetic, 0.004275200820433157);
	EXPECT_DOUBLE_EQ(stress(0,0), 5.2064533063673623e-06);
	EXPECT_DOUBLE_EQ(stress(0,1), -1.6467487572445481e-06);
	EXPECT_DOUBLE_EQ(stress(0,2), 1.5039983732220751e-06);
	EXPECT_DOUBLE_EQ(stress(1,0), -1.6467487572445481e-06);
	EXPECT_DOUBLE_EQ(stress(1,1), 2.3806464376131247e-06);
	EXPECT_DOUBLE_EQ(stress(1,2), -1.251414906590483e-06);
	EXPECT_DOUBLE_EQ(stress(2,0), 1.5039983732220751e-06);
	EXPECT_DOUBLE_EQ(stress(2,1), -1.251414906590483e-06);
	EXPECT_DOUBLE_EQ(stress(2,2), 9.6330189688582584e-07);

	delete[] allmass;
	delete[] pos;
	delete[] vel;
	delete[] ionmbl;
}

int main(int argc, char **argv) 
{
    srand(2000);
#ifdef __MPI
	MPI_Init(&argc,&argv);
#endif

    testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

#ifdef __MPI
    MPI_Finalize();
#endif

    return result;
}