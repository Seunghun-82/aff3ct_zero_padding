#include <sstream>
#include <streampu.hpp>

#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Tools/Codec/BCH/Codec_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename B, typename Q>
Codec_BCH<B, Q>::Codec_BCH(const factory::Encoder_BCH& enc_params, const factory::Decoder_BCH& dec_params)
  : Codec_SIHO<B, Q>(enc_params.K, enc_params.N_cw, enc_params.N_cw)
  , GF_poly(new BCH_polynomial_generator<B>(spu::tools::next_power_of_2(dec_params.N_cw) - 1, dec_params.t))
{
    // ----------------------------------------------------------------------------------------------------- exceptions
    if (enc_params.K != dec_params.K)
    {
        std::stringstream message;
        message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
                << ", 'dec_params.K' = " << dec_params.K << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    if (enc_params.N_cw != dec_params.N_cw)
    {
        std::stringstream message;
        message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
                << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    // ---------------------------------------------------------------------------------------------------- allocations
    factory::Puncturer pct_params;
    pct_params.type = "NO";
    pct_params.K = enc_params.K;
    pct_params.N = enc_params.N_cw;
    pct_params.N_cw = enc_params.N_cw;

    this->set_puncturer(pct_params.build<B, Q>());
    try
    {
        this->set_encoder(enc_params.build<B>(*GF_poly));
    }
    catch (spu::tools::cannot_allocate const&)
    {
        this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
    }
    if (dec_params.implem == "GENIUS") this->get_encoder().set_memorizing(true);
    this->set_decoder_siho(dec_params.build<B, Q>(*GF_poly, &this->get_encoder()));
}

template<typename B, typename Q>
Codec_BCH<B, Q>*
Codec_BCH<B, Q>::clone() const
{
    auto t = new Codec_BCH(*this);
    t->deep_copy(*this);
    return t;
}

template<typename B, typename Q>
const BCH_polynomial_generator<B>&
Codec_BCH<B, Q>::get_GF_poly() const
{
    return *this->GF_poly;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_BCH<B_8, Q_8>;
template class aff3ct::tools::Codec_BCH<B_16, Q_16>;
template class aff3ct::tools::Codec_BCH<B_32, Q_32>;
template class aff3ct::tools::Codec_BCH<B_64, Q_64>;
#else
template class aff3ct::tools::Codec_BCH<B, Q>;
#endif
// ==================================================================================== explicit template instantiation
