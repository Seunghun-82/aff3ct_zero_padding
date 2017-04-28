#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Factory/Polar/Factory_frozenbits_generator.hpp"
#include "Tools/Factory/Polar/Factory_encoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar.hpp"

#include "Simulation_BFERI_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFERI_polar<B,R,Q>
::Simulation_BFERI_polar(const parameters& params)
: Simulation_BFERI<B,R,Q>(params),
  frozen_bits(this->params.code.N_code),
  fb_generator(nullptr),
  decoder_siso(params.simulation.n_threads, nullptr)
{
	// build the frozen bits generator
	fb_generator = Factory_frozenbits_generator<B>::build(params);
	Simulation::check_errors(fb_generator, "Frozenbits_generator<B>");
}

template <typename B, typename R, typename Q>
Simulation_BFERI_polar<B,R,Q>
::~Simulation_BFERI_polar()
{
	if (fb_generator != nullptr) { delete fb_generator; fb_generator = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_polar<B,R,Q>
::launch_precompute()
{
	if (this->params.code.sigma != 0.f)
		fb_generator->generate(frozen_bits);
}

template <typename B, typename R, typename Q>
void Simulation_BFERI_polar<B,R,Q>
::snr_precompute()
{
	// adaptative frozen bits generation
	if (this->params.code.sigma == 0.f)
	{
		fb_generator->set_sigma(this->sigma);
		fb_generator->generate(frozen_bits);
	}

	if (this->params.simulation.debug)
	{
		std::clog << std::endl << "Frozen bits:" << std::endl;
		Frame_trace<B> ft(this->params.simulation.debug_limit);
		ft.display_bit_vector(frozen_bits);
		std::clog << std::endl;
	}
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFERI_polar<B,R,Q>
::build_encoder(const int tid)
{
	return Factory_encoder_polar<B>::build(this->params, frozen_bits);
}

template <typename B, typename R, typename Q>
SISO<Q>* Simulation_BFERI_polar<B,R,Q>
::build_siso(const int tid)
{
	decoder_siso[tid] = Factory_decoder_polar<B,Q>::build_siso(this->params, frozen_bits);
	return decoder_siso[tid];
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFERI_polar<B,R,Q>
::build_decoder(const int tid)
{
	return decoder_siso[tid];
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFERI_polar<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFERI_polar<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFERI_polar<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFERI_polar<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFERI_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
