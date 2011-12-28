#ifndef __NC_TYPETRAITS_h__
#define __NC_TYPETRAITS_h__

#include "Types.h"

namespace nc{

	namespace type_traits
	{

		//tr1 type traits

		template < typename T, T val >
		struct integral_constant{
			typedef T							value_type;
			typedef integral_constant< T, val >	type;
			static const T value = val;
		};

		typedef integral_constant< bool, true >  true_type;
		typedef integral_constant< bool, false > false_type;


		typedef uint8_t yes_type;
		struct no_type{
			uint8_t padding[4];
		};

		//ice = integral constant expression

		template <bool b1, bool b2, bool b3 = true, bool b4 = true, bool b5 = true, bool b6 = true, bool b7 = true>
		struct ice_and;

		template <bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7>
		struct ice_and{
			static const bool value = false;
		};

		template <>
		struct ice_and<true, true, true, true, true, true, true>{
			static const bool value = true;
		};

		template <int b1, int b2>
		struct ice_eq
		{
			static const bool value = (b1 == b2);
		};

		template <int b1, int b2>
		struct ice_ne
		{
			static const bool value = (b1 != b2);
		};

		template <bool b>
		struct ice_not
		{
			static const bool value = true;
		};

		template <>
		struct ice_not<true>
		{
			static const bool value = false;
		};


		template <bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false, bool b6 = false, bool b7 = false>
		struct ice_or;

		template <bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7>
		struct ice_or
		{
			static const bool value = true;
		};

		template <>
		struct ice_or<false, false, false, false, false, false, false>
		{
			static const bool value = false;
		};



		template< bool ConditionT, typename ThenT, typename ElseT >
		struct if_c;

		template< typename ThenT, typename ElseT >
		struct if_c< true, ThenT, ElseT >{
			static const bool	value = true;			
			typedef ThenT		type;
		};

		template< typename ThenT, typename ElseT >
		struct if_c< false, ThenT, ElseT >{
			static const bool	value = false;			
			typedef ElseT		type;
		};

		template< typename ConditionT, typename ThenT, typename ElseT >
		struct if_ : public if_c< ConditionT::value, ThenT, ElseT > {};


		//enmable_if, disable_if

		template < bool ConditionT, typename T = void >
		struct enable_if_c {
			typedef T type;
		};

		template < typename T >
		struct enable_if_c< false, T > {};

		template < typename ConditionT, typename T = void> 
		struct enable_if : public enable_if_c< ConditionT::value, T > {};

		template < bool ConditionT, typename T >
		struct lazy_enable_if_c {
			typedef typename T::type type;
		};

		template < typename T >
		struct lazy_enable_if_c< false, T > {};

		template < typename ConditionT, typename T > 
		struct lazy_enable_if : public lazy_enable_if_c< ConditionT::value, T > {};


		template < bool ConditionT, typename T = void >
		struct disable_if_c {
			typedef T type;
		};

		template < typename T >
		struct disable_if_c<true, T> {};

		template < typename ConditionT, typename T = void > 
		struct disable_if : public disable_if_c< ConditionT::value, T > {};

		template < bool ConditionT, typename T >
		struct lazy_disable_if_c {
			typedef typename T::type type;
		};

		template < typename T >
		struct lazy_disable_if_c< true, T > {};

		template < typename ConditionT, typename T > 
		struct lazy_disable_if : public lazy_disable_if_c< ConditionT::value, T > {};

		#define NC_BOOL_TRAITS_DEF1( TemplateParam, TraitClassName, Value) \
			template< TemplateParam > \
				struct TraitClassName : public integral_constant<bool, Value> {};

		#define NC_BOOL_TRAITS_SPEC1_0( TraitClassName, Special, Value) \
			template<> \
				struct TraitClassName< Special > : public integral_constant<bool, Value> {};

		#define NC_BOOL_TRAITS_SPEC1_1( TemplateParam, TraitClassName, Special, Value) \
			template< TemplateParam > \
				struct TraitClassName< Special > : public integral_constant<bool, Value> {};


		#define NC_BOOL_TRAITS_SPEC1_2( TemplateParam1, TemplateParam2, TraitClassName, Special, Value) \
			template< TemplateParam1, TemplateParam2 > \
				struct TraitClassName< Special > : public integral_constant<bool, Value> {};



		#define NC_BOOL_CV_TRAITS_SPEC1_0( TraitClassName, Special, Value) \
			NC_BOOL_TRAITS_SPEC1_0( TraitClassName, Special, Value )\
			NC_BOOL_TRAITS_SPEC1_0( TraitClassName, const Special, Value )\
			NC_BOOL_TRAITS_SPEC1_0( TraitClassName, volatile Special, Value )\
			NC_BOOL_TRAITS_SPEC1_0( TraitClassName, const volatile Special, Value )

		#define NC_BOOL_CV_TRAITS_SPEC1_1( TemplateParam, TraitClassName, Special, Value) \
			NC_BOOL_TRAITS_SPEC1_1( TemplateParam, TraitClassName, Special, Value )\
			NC_BOOL_TRAITS_SPEC1_1( TemplateParam, TraitClassName, const Special, Value )\
			NC_BOOL_TRAITS_SPEC1_1( TemplateParam, TraitClassName, volatile Special, Value )\
			NC_BOOL_TRAITS_SPEC1_1( TemplateParam, TraitClassName, const volatile Special, Value )

		#define NC_BOOL_CV_TRAITS_SPEC1_2( TemplateParam1, TemplateParam2, TraitClassName, Special, Value) \
			NC_BOOL_TRAITS_SPEC1_2( TemplateParam1, TemplateParam2, TraitClassName, Special, Value )\
			NC_BOOL_TRAITS_SPEC1_2( TemplateParam1, TemplateParam2, TraitClassName, const Special, Value )\
			NC_BOOL_TRAITS_SPEC1_2( TemplateParam1, TemplateParam2, TraitClassName, volatile Special, Value )\
			NC_BOOL_TRAITS_SPEC1_2( TemplateParam1, TemplateParam2, TraitClassName, const volatile Special, Value )



		#define NC_TYPE_TRAIT_DEF1( TraitClassName, T, ResultType ) \
			template< typename T > \
				struct TraitClassName { \
				typedef ResultType type; \
				};

		#define NC_TYPE_TRAIT_PARTIAL_SPEC1_1( TemplateParam, TraitClassName, Special, ResultType) \
			template< TemplateParam > \
				struct TraitClassName< Special > { \
				typedef ResultType type; \
				};

		#define NC_TYPE_TRAIT_PARTIAL_SPEC1_2( TemplateParam1, TemplateParam2, TraitClassName, Special, ResultType) \
			template< TemplateParam1, TemplateParam2 > \
				struct TraitClassName< Special > { \
				typedef ResultType type; \
				};


		// void型
		NC_BOOL_TRAITS_DEF1( typename T, is_void, false  )
		NC_BOOL_CV_TRAITS_SPEC1_0( is_void, void, false )


		// 整数型
		NC_BOOL_TRAITS_DEF1( typename T, is_integral, false  )
		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, bool,		true )
		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, char,		true)
		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, wchar_t,	true)

		//POD
		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, int8_t,		true)
		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, uint8_t,	true)

		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, int16_t,	true)
		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, uint16_t,	true)

		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, int32_t,	true)
		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, uint32_t,	true)

		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, int64_t,	true)
		NC_BOOL_CV_TRAITS_SPEC1_0( is_integral, uint64_t,	true)

		// 浮動小数点型
		NC_BOOL_TRAITS_DEF1( typename T, is_float, false  )
		NC_BOOL_CV_TRAITS_SPEC1_0( is_float, float,       true)
		NC_BOOL_CV_TRAITS_SPEC1_0( is_float, double,      true)
		NC_BOOL_CV_TRAITS_SPEC1_0( is_float, long double, true)


		//参照型
		NC_BOOL_TRAITS_DEF1( typename T, is_reference, false  )
		NC_BOOL_TRAITS_SPEC1_1( typename T, is_reference, T&,      true)

		// ポインタ型
		NC_BOOL_TRAITS_DEF1( typename T, is_pointer, false  )
		NC_BOOL_TRAITS_SPEC1_1( typename T, is_reference, T*,      true)


		// 浮動小数点型
		NC_BOOL_TRAITS_DEF1( typename T, is_array, false  )
		NC_BOOL_CV_TRAITS_SPEC1_2( typename T, size_t N,	is_array, T[N],		true )
		NC_BOOL_CV_TRAITS_SPEC1_1( typename T,				is_array, T[],		true )

		//Add and Remove-----------------


		//ポインタ付加
		NC_TYPE_TRAIT_DEF1( add_pointer,	T, T* )

		//const 付加
		NC_TYPE_TRAIT_DEF1( add_const,		T, const T )

		//volatile 付加
		NC_TYPE_TRAIT_DEF1( add_volatile,	T, volatile T )

		//const vlatile 付加
		NC_TYPE_TRAIT_DEF1( add_cv,			T, typename add_const< typename add_volatile< T >::type >::type )

		//参照 付加
		NC_TYPE_TRAIT_DEF1( add_reference,	T, T& )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, add_reference, T&, T )


		//const 削除
		NC_TYPE_TRAIT_DEF1( remove_const, T, T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_const, const T, T )

		//voaltile 削除
		NC_TYPE_TRAIT_DEF1( remove_volatile, T, T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_volatile, volatile T, T )

		//const volatile 削除
		NC_TYPE_TRAIT_DEF1( remove_cv, T, typename remove_volatile< typename remove_const< T >::type >::type )

		//参照型 削除
		NC_TYPE_TRAIT_DEF1( remove_reference, T, T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_reference, T&, T )

		//配列型 削除
		NC_TYPE_TRAIT_DEF1( remove_extent, T, T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_extent, T[],					T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_extent, T const[],			T const)
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_extent, T volatile[],			T volatile)
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_extent, T const volatile[],	T const volatile)
	

		//多次元配列の全てを型 削除
		NC_TYPE_TRAIT_DEF1( remove_all_extents, T, T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_all_extents, T[],					typename remove_all_extents< T >::type )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_all_extents, T const[],			typename remove_all_extents< T const >::type )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_all_extents, T volatile[],		typename remove_all_extents< T volatile >::type )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T, remove_all_extents, T const volatile[],	typename remove_all_extents< T const volatile >::type )


		//ポインタ型 削除
		NC_TYPE_TRAIT_DEF1( remove_pointer, T, T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T,remove_pointer, T*,					T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T,remove_pointer, T* const,				T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T,remove_pointer, T* volatile,			T )
		NC_TYPE_TRAIT_PARTIAL_SPEC1_1( typename T,remove_pointer, T* const volatile,	T )


		template< typename T >
		struct has_signed
			: integral_constant< bool, (typename remove_cv<T>::type)(-1) < (typename remove_cv<T>::type)(0) >
		{	// integral type can represent negative values
		};

		template< typename T >
		struct is_signed
			: integral_constant<	bool,
									is_float<T>::value ||
									is_integral<T>::value &&
									has_signed<
										typename if_< is_integral<T>, T, int>::type 
									>::value
								>
		{	// determine whether _Ty is a signed type
		};

		// TEMPLATE CLASS is_unsigned
		template< typename T >
		struct is_unsigned
			: integral_constant	<	bool,
									is_integral<T>::value &&
									!has_signed<
										typename if_< is_integral<T>, T, int>::type
									>::value
								>
		{	// determine whether _Ty is an unsigned type
		};


	}// namespace type_traits

/*

// is_class や is_union等は、開発環境のサポート無しでは動かない

namespace std { namespace tr1{

	using ::boost::integral_constant;
	using ::boost::true_type;
	using ::boost::false_type;
	using ::boost::is_void;
	using ::boost::is_integral;
	using ::boost::is_floating_point;
	using ::boost::is_array;
	using ::boost::is_pointer;
	using ::boost::is_reference;
using ::boost::is_member_object_pointer;
using ::boost::is_member_function_pointer;
//using ::boost::is_enum;
//using ::boost::is_union;
//using ::boost::is_class;
using ::boost::is_function;
using ::boost::is_arithmetic;
using ::boost::is_fundamental;
//using ::boost::is_object;
using ::boost::is_scalar;
using ::boost::is_compound;
using ::boost::is_member_pointer;
using ::boost::is_const;
using ::boost::is_volatile;
using ::boost::is_pod;
using ::boost::is_empty;
using ::boost::is_polymorphic;
using ::boost::is_abstract;
//using ::boost::has_trivial_constructor;
//using ::boost::has_trivial_copy;
//using ::boost::has_trivial_assign;
//using ::boost::has_trivial_destructor;
using ::boost::has_nothrow_constructor;
using ::boost::has_nothrow_copy;
using ::boost::has_nothrow_assign;
using ::boost::has_virtual_destructor;
using ::boost::is_signed;
using ::boost::is_unsigned;
using ::boost::alignment_of;
using ::boost::rank;
using ::boost::extent;
using ::boost::is_same;
using ::boost::tr1::is_base_of;
using ::boost::is_convertible;
	using ::boost::remove_const;
	using ::boost::remove_volatile;
	using ::boost::remove_cv;
	using ::boost::add_const;
	using ::boost::add_volatile;
	using ::boost::add_cv;
	using ::boost::remove_reference;
	using ::boost::add_reference;
	using ::boost::remove_extent;
	using ::boost::remove_all_extents;
	using ::boost::remove_pointer;
	using ::boost::add_pointer;
using ::boost::aligned_storage;

} }

*/


 

}//namespace nc


#endif // __NC_TYPETRAITS_h__