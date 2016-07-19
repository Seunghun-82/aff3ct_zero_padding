#ifndef MODULATOR_PAM_HPP_
#define MODULATOR_PAM_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"
#include "../Modulator.hpp"

template <typename B, typename R>
class Modulator_PAM : public Modulator<B,R>
{
private:
	const int bits_per_symbol;
	const int nbr_symbols;
	const R sigma;
	const R sqrt_es;
	mipp::vector<R> constellation;

public:
	Modulator_PAM(const int bits_per_symbol = 1, const R sigma = 1.0);
    virtual ~Modulator_PAM();

	virtual void   modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const;
	virtual void demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const;

	int get_buffer_size(const int N);

private:
	inline R bits_to_symbol(const B* bits) const;
};

#endif // MODULATOR_PAM_HPP_