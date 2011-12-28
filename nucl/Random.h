
#ifndef __NC_RANDOM_h__
#define __NC_RANDOM_h__

#include "Types.h"
#include "Assert.h"

namespace nc{
	
	/*!
	@class
		Random
	@brief
		make random value by XorShift algorithm.
		not thread safe.

		周期は、340正2823潤6692溝0938穣4634予6337京4607兆4317億6821万1455
		だけあるので、同じパターンが繰り返す心配はまずない。

		128 bit Xorshift は連続する４つの出力を得れば、
		将来だけでなく過去の数列も分かってしまうので、暗号には向かない。

		backは前のバージョンでは、最初に呼び出したときに、
		一つ前の乱数を返していたが、このバージョンでは最後の乱数を返す。
		そのほうが扱いやすい。
	*/
	class Random{

		uint32_t xor_seed[4];	//seeds

	public:

		//!< 整数型ランダムの最大値
		static const uint32_t RANDOM_MAX = _UI32_MAX;

		Random();

		//!< seedを設定
		void setSeed( uint32_t seed );

		//!< 0 <= N <= RANDOM_MAX 符号なし整数のランダム値を取得
		uint32_t rand();

		//!< 0 <= N <= max を満たす符号なし整数のランダム値を取得
		uint32_t rand( uint32_t max );

		//!	min <= N <= max を満たす符号なし整数のランダム値を取得
		//	min > max 時の動作は保証しない
		uint32_t rand( uint32_t min, uint32_t max );


		//! 0 <= N <= RANDOM_MAX 符号なし整数のランダム値を取得
		//	randの出力順序が逆なバージョン
		//	最初のrandBackの呼び出しは、直前のrand値を返します。
		uint32_t randBack();

		//! 0 <= N <= max を満たす符号なし整数のランダム値を取得
		//	randの出力順序が逆なバージョン
		//	最初のrandBackの呼び出しは、直前のrand値を返します。
		uint32_t randBack( uint32_t max );


		//! min <= N <= max を満たす符号なし整数のランダム値を取得
		//  min > max 時の動作は保証しない
		//	randの出力順序が逆なバージョン
		//	最初のrandBackの呼び出しは、直前のrand値を返します。
		uint32_t randBack( uint32_t min, uint32_t max );


		//!< 0.0f <= N <= 1.0f 浮動小数点一様乱数
		real_t frand();

		//! 0.0f <= N <= max <= 1.0f 浮動小数点一様乱数
		//	動作条件は 0 <= max <= 1.0f
		real_t frand( real_t max );

		//! min <= N  <= max  浮動小数点一様乱数
		//	min > max 時の動作は保証しない
		//	動作条件は 0 <= min < max <= 1.0f
		real_t frand( real_t min, real_t max );

		//!< 0.0f <= N <= 1.0f 浮動小数点一様乱数の逆順
		//	frandの出力順序が逆なバージョン
		//	最初のfrandBackの呼び出しは、直前のfrand値を返します。
		real_t frandBack();

		//!< 0.0f <= N <= max 浮動小数点一様乱数の逆順
		//	動作条件は 0 <= max <= 1.0f
		//	frandの出力順序が逆なバージョン
		//	最初のfrandBackの呼び出しは、直前のfrand値を返します。
		real_t frandBack( real_t max );


		//! min <= N  <= max  浮動小数点一様乱数
		//	min > max 時の動作は保証しない
		//	動作条件は 0 <= min < max <= 1.0f
		//	frandの出力順序が逆なバージョン
		//	最初のfrandBackの呼び出しは、直前のfrand値を返します。
		real_t frandBack( real_t min, real_t max );

	};



}//namespace nc

#endif // __NC_RANDOM_h__
