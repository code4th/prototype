#ifndef __NC_UNCOPYABLE_h__
#define __NC_UNCOPYABLE_h__

namespace nc{

	//�R�s�[�֎~�p�̃N���X��錾���邽�߂Ɏg�p����B
	//������΂Ɂ���Uncopyable*�ɃL���X�g���Ă͂����Ȃ��B
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