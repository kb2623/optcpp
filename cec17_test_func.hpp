#ifndef _CEC17_TEST_FUNC_H_
#define _CEC17_TEST_FUNC_H_

#include "objective_function.hpp"

#include <vector>

class CEC17: public ContiniousObjectiveFunciton {
private:
	double *OShift = nullptr, *M = nullptr;
	int *SS = nullptr;
	int ini_flag, n_flag, func_flag;

	void sphere_func (double *, double *, int , double *,double *, int, int); /* Sphere */
	void ellips_func(double *, double *, int , double *,double *, int, int); /* Ellipsoidal */
	void bent_cigar_func(double *, double *, int , double *,double *, int, int); /* Discus */
	void discus_func(double *, double *, int , double *,double *, int, int);  /* Bent_Cigar */
	void dif_powers_func(double *, double *, int , double *,double *, int, int);  /* Different Powers */
	void rosenbrock_func (double *, double *, int , double *,double *, int, int); /* Rosenbrock's */
	void schaffer_F7_func (double *, double *, int , double *,double *, int, int); /* Schwefel's F7 */
	void ackley_func (double *, double *, int , double *,double *, int, int); /* Ackley's */
	void rastrigin_func (double *, double *, int , double *,double *, int, int); /* Rastrigin's  */
	void weierstrass_func (double *, double *, int , double *,double *, int, int); /* Weierstrass's  */
	void griewank_func (double *, double *, int , double *,double *, int, int); /* Griewank's  */
	void schwefel_func (double *, double *, int , double *,double *, int, int); /* Schwefel's */
	void katsuura_func (double *, double *, int , double *,double *, int, int); /* Katsuura */
	void bi_rastrigin_func (double *, double *, int , double *,double *, int, int); /* Lunacek Bi_rastrigin */
	void grie_rosen_func (double *, double *, int , double *,double *, int, int); /* Griewank-Rosenbrock  */
	void escaffer6_func (double *, double *, int , double *,double *, int, int); /* Expanded Scaffer¡¯s F6  */
	void step_rastrigin_func (double *, double *, int , double *,double *, int, int); /* Noncontinuous Rastrigin's  */
	void happycat_func (double *, double *, int , double *,double *, int, int); /* HappyCat */
	void hgbat_func (double *, double *, int , double *,double *, int, int); /* HGBat  */

	/* New functions Noor Changes */
	void sum_diff_pow_func(double *, double *, int , double *,double *, int, int); /* Sum of different power */
	void zakharov_func(double *, double *, int , double *,double *, int, int); /* ZAKHAROV */
	void levy_func(double *, double *, int , double *,double *, int, int); /* Levy */
	void dixon_price_func(double *, double *, int , double *,double *, int, int); /* Dixon and Price */

	void hf01 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 1 */
	void hf02 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 2 */
	void hf03 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 3 */
	void hf04 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 4 */
	void hf05 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 5 */
	void hf06 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 6 */
	void hf07 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 7 */
	void hf08 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 8 */
	void hf09 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 9 */
	void hf10 (double *, double *, int, double *,double *, int *,int, int); /* Hybrid Function 10 */

	void cf01 (double *, double *, int , double *,double *, int); /* Composition Function 1 */
	void cf02 (double *, double *, int , double *,double *, int); /* Composition Function 2 */
	void cf03 (double *, double *, int , double *,double *, int); /* Composition Function 3 */
	void cf04 (double *, double *, int , double *,double *, int); /* Composition Function 4 */
	void cf05 (double *, double *, int , double *,double *, int); /* Composition Function 5 */
	void cf06 (double *, double *, int , double *,double *, int); /* Composition Function 6 */
	void cf07 (double *, double *, int , double *,double *, int); /* Composition Function 7 */
	void cf08 (double *, double *, int , double *,double *, int); /* Composition Function 8 */
	void cf09 (double *, double *, int , double *,double *, int *, int); /* Composition Function 9 */
	void cf10 (double *, double *, int , double *,double *, int *, int); /* Composition Function 10 */

	void sr_func (double *, double *, double *, int, double*, double*, double, int, int); /* shift and rotate */
	void cf_cal (double *, double *, double *,int, double *,double *,double *,double *,int);

public:
	CEC17(int, int);
	~CEC17();

	virtual double func (double*, int) override;
};

void shiftfunc (double *, double *, int, double *);
void rotatefunc (double *, double *, int, double *);

#endif
