#include <cpp11.hpp>

#include <R_ext/Altrep.h>

// -----------------------------------------------------------------------------

R_altrep_class_t fails_t;

R_xlen_t fails_Length(SEXP x) {
  // Length method is required at a minimum
  return 1;
}

double fails_Elt(SEXP x, R_xlen_t i) {
  // When the `Elt` method is hit, error through cpp11.
  // Goes through `unwind_protect()`.
  cpp11::stop("oh no");
  return 0;
}

[[cpp11::register]]
SEXP make_fails() {
  return R_new_altrep(fails_t, R_NilValue, R_NilValue);
}

// -----------------------------------------------------------------------------

R_altrep_class_t works_t;

R_xlen_t works_Length(SEXP x) {
  // Length method is required at a minimum
  return 1;
}

double works_Elt(SEXP x, R_xlen_t i) {
  // When the `Elt` method is hit, error through base R
  Rf_error("oh no");
  return 0;
}

[[cpp11::register]]
SEXP make_works() {
  return R_new_altrep(works_t, R_NilValue, R_NilValue);
}

// -----------------------------------------------------------------------------

[[cpp11::init]]
void init_altrep(DllInfo* dll) {
  fails_t = R_make_altreal_class("fails_t", "cpp11altrepbug", dll);
  R_set_altrep_Length_method(fails_t, fails_Length);
  R_set_altreal_Elt_method(fails_t, fails_Elt);

  works_t = R_make_altreal_class("works_t", "cpp11altrepbug", dll);
  R_set_altrep_Length_method(works_t, works_Length);
  R_set_altreal_Elt_method(works_t, works_Elt);
}
