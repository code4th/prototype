#ifndef __NC_UNCOPYABLE_h__
#define __NC_UNCOPYABLE_h__

namespace nc{

	//コピー禁止用のクラスを宣言するために使用する。
	//＊＊絶対に＊＊Uncopyable*にキャストしてはいけない。
	//cuz, Uncopyable haven't virtual destructor.

	class Uncopyable{
	protected:
		Uncopyable() {}
		~Uncopyable() {}
	private:
		Uncopyable( const Uncopyable& );
		Uncopyable& operator = ( const Uncopyable& );
	};

}//namespace nc

#endif // __NC_UNCOPYABLE_h__