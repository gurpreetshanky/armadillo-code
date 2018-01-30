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


//! \addtogroup op_chi2rnd
//! @{



template<typename T1>
inline
void
op_chi2rnd::apply(Mat<typename T1::elem_type>& out, const Op<T1,op_chi2rnd>& in)
  {
  arma_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const Proxy<T1> P(in.m);
  
  if(P.is_alias(out) == false)
    {
    op_chi2rnd::apply_noalias(out, P);
    }
  else
    {
    Mat<eT> tmp;
    
    op_chi2rnd::apply_noalias(tmp, P);
    
    out.steal_mem(tmp);
    }
  }



template<typename T1>
inline
void
op_chi2rnd::apply_noalias(Mat<typename T1::elem_type>& out, const Proxy<T1>& P)
  {
  arma_extra_debug_sigprint();
  
  #if defined(ARMA_USE_CXX11)
    {
    typedef typename T1::elem_type eT;
    
    op_chi2rnd_generator<eT> generator;
    
    const uword n_rows = P.get_n_rows();
    const uword n_cols = P.get_n_cols();
    
    out.set_size(n_rows, n_cols);
    
    eT* out_mem = out.memptr();
    
    if(Proxy<T1>::use_at == false)
      {
      const uword N = P.get_n_elem();
      
      typename Proxy<T1>::ea_type Pea = P.get_ea();
      
      for(uword i=0; i<N; ++i)
        {
        out_mem[i] = generator( Pea[i] );
        }
      }
    else
      {
      for(uword col=0; col < n_cols; ++col)
      for(uword row=0; row < n_rows; ++row)
        {
        (*out_mem) = generator( P.at(row,col) );  ++out_mem;
        }
      }
    }
  #else
    {
    arma_stop_logic_error("chi2rnd(): C++11 compiler required");
    
    out.reset();
    }
  #endif
  }



//



#if defined(ARMA_USE_CXX11)

template<typename eT>
inline
op_chi2rnd_generator<eT>::~op_chi2rnd_generator()
  {
  arma_extra_debug_sigprint();
  }



template<typename eT>
inline
op_chi2rnd_generator<eT>::op_chi2rnd_generator()
  {
  arma_extra_debug_sigprint();
  
  typedef std::mt19937_64::result_type seed_type;
  
  motor.seed( seed_type(arma_rng::randi<int>()) );
  }



template<typename eT>
inline
eT
op_chi2rnd_generator<eT>::operator()(const eT val)
  {
  arma_extra_debug_sigprint();
  
  // as C++11 doesn't seem to provide a way to explicitly set the parameter
  // of an existing chi_squared_distribution object,
  // we need to create a new object each time
  
  if(val > eT(0))
    {
    std::chi_squared_distribution<eT> distr(val);
    
    return eT( distr(motor) );
    }
  else
    {
    return Datum<eT>::nan;
    }
  }

#endif



//! @}
