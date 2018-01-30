// Copyright 2008-2016 Conrad Sanderson (http://conradsanderson.id.au)
// Copyright 2008-2016 National ICT Australia (NICTA)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------


//! \addtogroup fn_chi2rnd
//! @{



arma_warn_unused
inline
double
chi2rnd(const double df)
  {
  arma_extra_debug_sigprint();
  
  #if defined(ARMA_USE_CXX11)
    {
    op_chi2rnd_generator<double> generator;
    
    return generator(df);
    }
  #else
    {
    arma_stop_logic_error("chi2rnd(): C++11 compiler required");
    
    return double(0);
    }
  #endif
  }



template<typename T1>
arma_warn_unused
inline
typename
enable_if2
  <
  (is_arma_type<T1>::value && is_real<typename T1::elem_type>::value),
  const Op<T1, op_chi2rnd>
  >::result
chi2rnd(const T1& expr)
  {
  arma_extra_debug_sigprint();
  
  return Op<T1, op_chi2rnd>(expr);
  }



// TODO: overload chi2rnd() to accept constant val and size specification



//! @}
