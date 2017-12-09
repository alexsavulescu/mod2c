/* Created by Language version: 6.2.0 */
/* NOT VECTORIZED */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "coreneuron/mech/cfile/scoplib.h"
#undef PI
 
#include "coreneuron/utils/randoms/nrnran123.h"
#include "coreneuron/nrnoc/md1redef.h"
#include "coreneuron/nrnconf.h"
#include "coreneuron/nrnoc/multicore.h"
#include "coreneuron/nrniv/nrn_acc_manager.h"
#include "coreneuron/mech/cfile/scoplib.h"

#include "coreneuron/nrnoc/md2redef.h"
#if METHOD3
extern int _method3;
#endif

#if !NRNGPU
#if !defined(DISABLE_HOC_EXP)
#undef exp
#define exp hoc_Exp
#endif
extern double hoc_Exp(double);
#endif
 
#undef LAYOUT
#define LAYOUT 1
#define _STRIDE 1
 
#define nrn_init _nrn_init__napIn
#define nrn_cur _nrn_cur__napIn
#define _nrn_current _nrn_current__napIn
#define nrn_jacob _nrn_jacob__napIn
#define nrn_state _nrn_state__napIn
#define initmodel initmodel__napIn
#define _net_receive _net_receive__napIn
#define nrn_state_launcher nrn_state_napIn_launcher
#define nrn_cur_launcher nrn_cur_napIn_launcher
#define nrn_jacob_launcher nrn_jacob_napIn_launcher 
#define states states_napIn 
#define trates trates_napIn 
 
#define _threadargscomma_ /**/
#define _threadargsprotocomma_ /**/
#define _threadargs_ /**/
#define _threadargsproto_ /**/
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 static double *_p; static Datum *_ppvar;
 
#define t nrn_threads->_t
#define dt nrn_threads->_dt
#define gbar _p[0*_STRIDE]
#define htau _p[1*_STRIDE]
#define thegna _p[2*_STRIDE]
#define m _p[3*_STRIDE]
#define h _p[4*_STRIDE]
#define ena _p[5*_STRIDE]
#define ina _p[6*_STRIDE]
#define Dm _p[7*_STRIDE]
#define Dh _p[8*_STRIDE]
#define _g _p[9*_STRIDE]
#define _ion_ena		_nt_data[_ppvar[0*_STRIDE]]
#define _ion_ina	_nt_data[_ppvar[1*_STRIDE]]
#define _ion_dinadv	_nt_data[_ppvar[2*_STRIDE]]
 
#if MAC
#if !defined(v)
#define v _mlhv
#endif
#if !defined(h)
#define h _mlhh
#endif
#endif
 
#if defined(__cplusplus)
extern "C" {
#endif
 static int hoc_nrnpointerindex =  -1;
 /* external NEURON variables */
 extern double celsius;
 #if defined(PG_ACC_BUGS)
#define _celsius_ _celsius__napIn
double _celsius_;
#pragma acc declare copyin(_celsius_)
#define celsius _celsius_
#endif
 
#if 0 /*BBCORE*/
 /* declaration of user functions */
 static void _hoc_trates(void);
 
#endif /*BBCORE*/
 static int _mechtype;
 extern int nrn_get_mechtype();
extern void hoc_register_prop_size(int, int, int);
extern Memb_func* memb_func;
 
#if 0 /*BBCORE*/
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 "setdata_napIn", _hoc_setdata,
 "trates_napIn", _hoc_trates,
 0, 0
};
 
#endif /*BBCORE*/
 /* declare global and static user variables */
#define eNa eNa_napIn
 double eNa = 55;
 #pragma acc declare copyin (eNa)
#define hinf hinf_napIn
 double hinf = 0;
 #pragma acc declare copyin (hinf)
#define mtau mtau_napIn
 double mtau = 0;
 #pragma acc declare copyin (mtau)
#define minf minf_napIn
 double minf = 0;
 #pragma acc declare copyin (minf)
 
static void _acc_globals_update() {
 #pragma acc update device (eNa) if(nrn_threads->compute_gpu)
 #pragma acc update device (hinf) if(nrn_threads->compute_gpu)
 #pragma acc update device (mtau) if(nrn_threads->compute_gpu)
 #pragma acc update device (minf) if(nrn_threads->compute_gpu)
 }
 
#if 0 /*BBCORE*/
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "eNa_napIn", "mV",
 "mtau_napIn", "ms",
 "gbar_napIn", "mho/cm2",
 "htau_napIn", "ms",
 "thegna_napIn", "mho/cm2",
 0,0
};
 
#endif /*BBCORE*/
 static double delta_t = 0.01;
 static double h0 = 0;
 static double m0 = 0;
 static double v = 0;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "eNa_napIn", &eNa_napIn,
 "minf_napIn", &minf_napIn,
 "hinf_napIn", &hinf_napIn,
 "mtau_napIn", &mtau_napIn,
 0,0
};
 static DoubVec hoc_vdoub[] = {
 0,0,0
};
 static double _sav_indep;
 static void nrn_alloc(double*, Datum*, int);
void nrn_init(_NrnThread*, _Memb_list*, int);
void nrn_state(_NrnThread*, _Memb_list*, int);
 void nrn_cur(_NrnThread*, _Memb_list*, int);
 void nrn_jacob(_NrnThread*, _Memb_list*, int);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "6.2.0",
"napIn",
 "gbar_napIn",
 "htau_napIn",
 0,
 "thegna_napIn",
 0,
 "m_napIn",
 "h_napIn",
 0,
 0};
 static int _na_type;
 
static void nrn_alloc(double* _p, Datum* _ppvar, int _type) {
 
#if 0 /*BBCORE*/
 	/*initialize range parameters*/
 	gbar = 0.0052085;
 	htau = 15;
 prop_ion = need_memb(_na_sym);
 nrn_promote(prop_ion, 0, 1);
 	_ppvar[0]._pval = &prop_ion->param[0]; /* ena */
 	_ppvar[1]._pval = &prop_ion->param[3]; /* ina */
 	_ppvar[2]._pval = &prop_ion->param[4]; /* _ion_dinadv */
 
#endif /* BBCORE */
 
}
 static void _initlists();
 static void _update_ion_pointer(Datum*);
 
#define _psize 10
#define _ppsize 3
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*f)(Datum*));
extern void _nrn_thread_table_reg(int, void(*)(_threadargsproto_, int));
extern void _cvode_abstol( Symbol**, double*, int);

 void _NapIn_reg() {
	int _vectorized = 0;
  _initlists();
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 if (_mechtype == -1) return;
 _nrn_layout_reg(_mechtype, LAYOUT);
 _na_type = nrn_get_mechtype("na_ion"); 
#if 0 /*BBCORE*/
 	ion_reg("na", -10000.);
 	_na_sym = hoc_lookup("na_ion");
 
#endif /*BBCORE*/
 	register_mech(_mechanism, nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init, hoc_nrnpointerindex, 0);
  hoc_register_prop_size(_mechtype, _psize, _ppsize);
  hoc_register_dparam_semantics(_mechtype, 0, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 2, "na_ion");
 	hoc_register_var(hoc_scdoub, hoc_vdoub, NULL);
 }
static int _reset;
static char *modelname = "nap";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int trates(double);
 
static int _ode_spec1(_threadargsproto_);
/*static int _ode_matsol1(_threadargsproto_);*/
 
#define _slist1 _slist1_napIn
int* _slist1;
#pragma acc declare create(_slist1)

#define _dlist1 _dlist1_napIn
int* _dlist1;
#pragma acc declare create(_dlist1)
 static inline int states(_threadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 () {_reset=0;
 {
   trates ( _threadargscomma_ v ) ;
   Dm = ( minf - m ) / mtau ;
   Dh = ( hinf - h ) / htau ;
   }
 return _reset;
}
 static int _ode_matsol1 () {
 trates ( _threadargscomma_ v ) ;
 Dm = Dm  / (1. - dt*( ( ( ( - 1.0 ) ) ) / mtau )) ;
 Dh = Dh  / (1. - dt*( ( ( ( - 1.0 ) ) ) / htau )) ;
 return 0;
}
 /*END CVODE*/
 static int states () {_reset=0;
 {
   trates ( _threadargscomma_ v ) ;
    m = m + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / mtau)))*(- ( ( ( minf ) ) / mtau ) / ( ( ( ( - 1.0) ) ) / mtau ) - m) ;
    h = h + (1. - exp(dt*(( ( ( - 1.0 ) ) ) / htau)))*(- ( ( ( hinf ) ) / htau ) / ( ( ( ( - 1.0) ) ) / htau ) - h) ;
   }
  return 0;
}
 
static int  trates (  double _lvm ) {
   mtau = 1.0 ;
   minf = 1.0 / ( 1.0 + exp ( - ( v + 52.3 ) / 6.8 ) ) ;
   hinf = 1.0 / ( 1.0 + exp ( ( v + 48.0 ) / 10.0 ) ) ;
    return 0; }
 
#if 0 /*BBCORE*/
 
static void _hoc_trates(void) {
  double _r;
   _r = 1.;
 trates (  *getarg(1) ;
 hoc_retpushx(_r);
}
 
#endif /*BBCORE*/
 static void _update_ion_pointer(Datum* _ppvar) {
 }

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
  h = h0;
  m = m0;
 {
   trates ( _threadargscomma_ v ) ;
   m = minf ;
   h = hinf ;
   }
  _sav_indep = t; t = _save;

}
}

static void nrn_init(_NrnThread* _nt, _Memb_list* _ml, int _type){
double _v; int* _ni; int _iml, _cntml_padded, _cntml_actual;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml_actual = _ml->_nodecount;
_cntml_padded = _ml->_nodecount_padded;
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
 _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;
    _v = _vec_v[_nd_idx];
    _PRCELLSTATE_V
 v = _v;
  ena = _ion_ena;
 initmodel();
 }}

static double _nrn_current(double _v){double _current=0.;v=_v;{ {
   trates ( _threadargscomma_ v ) ;
   thegna = gbar * m * h ;
   ina = thegna * ( v - eNa ) ;
   }
 _current += ina;

} return _current;
}

static void nrn_cur(_NrnThread* _nt, _Memb_list* _ml, int _type){
int* _ni; double _rhs, _v; int _iml, _cntml_padded, _cntml_actual;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml_actual = _ml->_nodecount;
_cntml_padded = _ml->_nodecount_padded;
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
 _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;
    _v = _vec_v[_nd_idx];
    _PRCELLSTATE_V
  ena = _ion_ena;
 _g = _nrn_current(_v + .001);
 	{ double _dina;
  _dina = ina;
 _rhs = _nrn_current(_v);
  _ion_dinadv += (_dina - ina)/.001 ;
 	}
 _g = (_g - _rhs)/.001;
  _ion_ina += ina ;
	VEC_RHS(_ni[_iml]) -= _rhs;
 
}}

static void nrn_jacob(_NrnThread* _nt, _Memb_list* _ml, int _type){
int* _ni; int _iml, _cntml_padded, _cntml_actual;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml_actual = _ml->_nodecount;
_cntml_padded = _ml->_nodecount_padded;
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
 _p = _ml->_data + _iml*_psize;
	VEC_D(_ni[_iml]) += _g;
 
}}

static void nrn_state(_NrnThread* _nt, _Memb_list* _ml, int _type){
double _v = 0.0; int* _ni; int _iml, _cntml_padded, _cntml_actual;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml_actual = _ml->_nodecount;
_cntml_padded = _ml->_nodecount_padded;
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
 _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;
    _v = _vec_v[_nd_idx];
    _PRCELLSTATE_V
 v=_v;
 _PRCELLSTATE_V
{
  ena = _ion_ena;
 { error =  states();
 if(error){fprintf(stderr,"at line 43 in file NapIn.mod:\n    		\n"); nrn_complain(_p); abort_run(error);}
 } }}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
 int _cntml_actual=1;
 int _cntml_padded=1;
 int _iml=0;
  if (!_first) return;
 
 _slist1 = (int*)malloc(sizeof(int)*2);
 _dlist1 = (int*)malloc(sizeof(int)*2);
 _slist1[0] = &(m) - _p;  _dlist1[0] = &(Dm) - _p;
 _slist1[1] = &(h) - _p;  _dlist1[1] = &(Dh) - _p;
 #pragma acc enter data copyin(_slist1[0:2])
 #pragma acc enter data copyin(_dlist1[0:2])

_first = 0;
}
