#include <GarrysMod/Lua/Interface.h>
#include <mathlib/vmatrix.h>
#include <mathlib/vector.h>

#define LUA_FUNCTION( name ) int name( lua_State *state )

#define GET_MATRIX( index ) ( (VMatrix *)( (GarrysMod::Lua::UserData *)LUA->GetUserdata( index ) )->data )
#define GET_VECTOR( index ) ( (Vector *)( (GarrysMod::Lua::UserData *)LUA->GetUserdata( index ) )->data )

#define PUSH_MATRIX( matrix )																									\
	GarrysMod::Lua::UserData *userdata = (GarrysMod::Lua::UserData *)LUA->NewUserdata( sizeof( GarrysMod::Lua::UserData ) );	\
	userdata->data = matrix;																									\
	userdata->type = GarrysMod::Lua::Type::MATRIX;																				\
	LUA->CreateMetaTableType( "VMatrix", GarrysMod::Lua::Type::MATRIX );														\
	LUA->SetMetaTable( -2 );
#define PUSH_VECTOR( vector )																									\
	GarrysMod::Lua::UserData *userdata = (GarrysMod::Lua::UserData *)LUA->NewUserdata( sizeof( GarrysMod::Lua::UserData ) );	\
	userdata->data = vector;																									\
	userdata->type = GarrysMod::Lua::Type::VECTOR;																				\
	LUA->CreateMetaTableType( "Vector", GarrysMod::Lua::Type::VECTOR );															\
	LUA->SetMetaTable( -2 );

LUA_FUNCTION( lMatrix__add )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );
	LUA->CheckType( 2, GarrysMod::Lua::Type::MATRIX );

	PUSH_MATRIX( new VMatrix( *GET_MATRIX( 1 ) + *GET_MATRIX( 2 ) ) );
	return 1;
}

LUA_FUNCTION( lMatrix__mul )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	int type = LUA->GetType( 2 );
	switch( type )
	{
		case GarrysMod::Lua::Type::VECTOR:
		{
			PUSH_VECTOR( new Vector( *GET_MATRIX( 1 ) * *GET_VECTOR( 2 ) ) );
			return 1;
		}

		case GarrysMod::Lua::Type::MATRIX:
		{
			PUSH_MATRIX( new VMatrix( *GET_MATRIX( 1 ) * *GET_MATRIX( 2 ) ) );
			return 1;
		}
	}

	char err[80];
	sprintf( err, "invalid object to #2 (Vector or VMatrix expected, got %s)", LUA->GetTypeName( type ) );
	LUA->ThrowError( err );
	return 0;
}

LUA_FUNCTION( lMatrix__sub )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );
	LUA->CheckType( 2, GarrysMod::Lua::Type::MATRIX );

	PUSH_MATRIX( new VMatrix( *GET_MATRIX( 1 ) - *GET_MATRIX( 2 ) ) );
	return 1;
}

LUA_FUNCTION( lMatrix__eq )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	if( LUA->GetType( 2 ) == GarrysMod::Lua::Type::MATRIX )
	{
		LUA->PushBool( *GET_MATRIX( 1 ) == *GET_MATRIX( 2 ) );
		return 1;
	}

	LUA->PushBool( false );
	return 1;
}

LUA_FUNCTION( lMatrix__unm )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	PUSH_MATRIX( new VMatrix( -*GET_MATRIX( 1 ) ) );
	return 1;
}

LUA_FUNCTION( lMatrix__tostring )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	vec_t( *vec_tMat )[4] = GET_MATRIX( 1 )->m;
	char ret[256];
	sprintf(
		ret, 
		"[%.5f,\t%.5f,\t%.5f,\t%.5f]\n[%.5f,\t%.5f,\t%.5f,\t%.5f]\n[%.5f,\t%.5f,\t%.5f,\t%.5f]\n[%.5f,\t%.5f,\t%.5f,\t%.5f]", 
		vec_tMat[0][0],
		vec_tMat[0][1],
		vec_tMat[0][2],
		vec_tMat[0][3],

		vec_tMat[1][0],
		vec_tMat[1][1],
		vec_tMat[1][2],
		vec_tMat[1][3],

		vec_tMat[2][0],
		vec_tMat[2][1],
		vec_tMat[2][2],
		vec_tMat[2][3],

		vec_tMat[3][0],
		vec_tMat[3][1],
		vec_tMat[3][2],
		vec_tMat[3][3]
	);

	LUA->PushString( ret );
	return 1;
}

LUA_FUNCTION( lMatrixInverse )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	VMatrix *temp = GET_MATRIX( 1 );
	LUA->PushBool( temp->InverseGeneral( *temp ) );
	return 1;
}

LUA_FUNCTION( lMatrixGetInverse )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	VMatrix *retMat = new VMatrix( );
	if( !GET_MATRIX( 1 )->InverseGeneral( *retMat ) )
	{
		delete retMat;
		LUA->PushNil( );
		return 1;
	}

	PUSH_MATRIX( retMat );
	return 1;
}

LUA_FUNCTION( lMatrixIsRotationMatrix )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	LUA->PushBool( GET_MATRIX( 1 )->IsRotationMatrix( ) );
	return 1;
}

LUA_FUNCTION( lMatrixIsIdentity )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	LUA->PushBool( GET_MATRIX( 1 )->IsIdentity( ) );
	return 1;
}

LUA_FUNCTION( lMatrixIdentity )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	GET_MATRIX( 1 )->Identity( );
	return 0;
}

LUA_FUNCTION( lMatrixTranspose )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	VMatrix *temp = GET_MATRIX( 1 );
	MatrixTranspose( *temp, *temp );
	return 0;
}

LUA_FUNCTION( lMatrixGetTranspose )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	VMatrix *retMat = new VMatrix( );
	MatrixTranspose( *GET_MATRIX( 1 ), *retMat );
	PUSH_MATRIX( retMat );
	return 1;
}

LUA_FUNCTION( lMatrixGetField )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );
	LUA->CheckType( 2, GarrysMod::Lua::Type::NUMBER );
	LUA->CheckType( 3, GarrysMod::Lua::Type::NUMBER );

	int row = (int)LUA->GetNumber( 2 );
	if( row > 4 || row < 1 )
	{
		LUA->ThrowError( "invalid row" );
	}

	int col = (int)LUA->GetNumber( 3 );
	if( col > 4 || col < 1 )
	{
		LUA->ThrowError( "invalid column" );
	}

	LUA->PushNumber( (double)GET_MATRIX( 1 )->m[row][col] );
	return 1;
}

LUA_FUNCTION( lMatrixSetField )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );
	LUA->CheckType( 2, GarrysMod::Lua::Type::NUMBER );
	LUA->CheckType( 3, GarrysMod::Lua::Type::NUMBER );
	LUA->CheckType( 4, GarrysMod::Lua::Type::NUMBER );

	int row = (int)LUA->GetNumber( 2 );
	if( row > 4 || row < 1 )
	{
		LUA->ThrowError( "invalid row" );
	}

	int col = (int)LUA->GetNumber( 3 );
	if( col > 4 || col < 1 )
	{
		LUA->ThrowError( "invalid column" );
	}

	GET_MATRIX( 1 )->m[row][col] = (float)LUA->GetNumber( 4 );
	return 0;
}

#define createMatGetDir( dir )													\
LUA_FUNCTION( lMatrixGet##dir )													\
{																				\
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );							\
	PUSH_VECTOR( new Vector( GET_MATRIX( 1 )->Get##dir( ) ) );					\
	return 1;																	\
}

createMatGetDir( Forward );
createMatGetDir( Left );
createMatGetDir( Up );

#define createMatSetDir(dir)							\
LUA_FUNCTION( lMatrixSet ## dir ) {						\
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );	\
	LUA->CheckType( 2, GarrysMod::Lua::Type::VECTOR );	\
	GET_MATRIX( 1 )->Set##dir( *GET_VECTOR( 2 ) );		\
	return 0;											\
}

createMatSetDir( Forward );
createMatSetDir( Left );
createMatSetDir( Up );

LUA_FUNCTION( lMatrixSet )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );
	LUA->CheckType( 2, GarrysMod::Lua::Type::MATRIX );

	MatrixCopy( *GET_MATRIX( 2 ), *GET_MATRIX( 1 ) );
	return 0;
}

LUA_FUNCTION( lMatrixCopy )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	PUSH_MATRIX( new VMatrix( *GET_MATRIX( 1 ) ) );
	return 1;
}

LUA_FUNCTION( lMatrixToTable )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );

	vec_t( *vec_tMat )[4] = GET_MATRIX( 1 )->m;
	LUA->CreateTable( );
	for( int i = 1; 5 > i; i++ )
	{
		LUA->PushNumber( (double)i );
		LUA->CreateTable( );

			LUA->PushNumber( 1.0 );
			LUA->PushNumber( (double)vec_tMat[i - 1][0] );
			LUA->SetTable( -3 );

			LUA->PushNumber( 2.0 );
			LUA->PushNumber( (double)vec_tMat[i - 1][1] );
			LUA->SetTable( -3 );

			LUA->PushNumber( 3.0 );
			LUA->PushNumber( (double)vec_tMat[i - 1][2] );
			LUA->SetTable( -3 );

			LUA->PushNumber( 4.0 );
			LUA->PushNumber( (double)vec_tMat[i - 1][3] );
			LUA->SetTable( -3 );

		LUA->SetTable( -3 );
	}

	return 1;
}

LUA_FUNCTION( lMatrixFromTable )
{
	LUA->CheckType( 1, GarrysMod::Lua::Type::MATRIX );
	LUA->CheckType( 2, GarrysMod::Lua::Type::TABLE );

	VMatrix *mat = GET_MATRIX( 1 );
	vec_t( *vec_tMat )[4] = mat->m;

	LUA->Push( 2 );
	for( int i = 1; 5 > i; i++ )
	{
		LUA->PushNumber( (double)i );
		LUA->GetTable( -2 );
		if( !LUA->IsType( -1, GarrysMod::Lua::Type::TABLE ) )
		{
			LUA->ThrowError( "main table member not table!" );
		}

		for( int k = 1; 5 > k; k++ )
		{
			LUA->PushNumber( (double)k );
			LUA->GetTable( -2 );
			if( !LUA->IsType( -1, GarrysMod::Lua::Type::NUMBER ) )
			{
				LUA->ThrowError( "subtable table member not number!" );
			}

			vec_tMat[i - 1][k - 1] = (float)LUA->GetNumber( );
		}
	}

	return 0;
}

GMOD_MODULE_OPEN( )
{
	LUA->CreateMetaTableType( "VMatrix", GarrysMod::Lua::Type::MATRIX );

		LUA->PushCFunction( lMatrix__add );
		LUA->SetField( -2, "__add" );

		LUA->PushCFunction( lMatrix__eq );
		LUA->SetField( -2, "__eq" );

		LUA->PushCFunction( lMatrix__mul );
		LUA->SetField( -2, "__mul" );

		LUA->PushCFunction( lMatrix__sub );
		LUA->SetField( -2, "__sub" );

		LUA->PushCFunction( lMatrix__unm );
		LUA->SetField( -2, "__unm" );

		LUA->PushCFunction( lMatrix__tostring );
		LUA->SetField( -2, "__tostring" );

		LUA->PushCFunction( lMatrixInverse );
		LUA->SetField( -2, "Inverse" );

		LUA->PushCFunction( lMatrixGetInverse );
		LUA->SetField( -2, "GetInverse" );

		LUA->PushCFunction( lMatrixIsRotationMatrix );
		LUA->SetField( -2, "IsRotationMatrix" );

		LUA->PushCFunction( lMatrixIsIdentity );
		LUA->SetField( -2, "IsIdentity" );

		LUA->PushCFunction( lMatrixIdentity );
		LUA->SetField( -2, "Identity" );

		LUA->PushCFunction( lMatrixTranspose );
		LUA->SetField( -2, "Transpose" );

		LUA->PushCFunction( lMatrixGetTranspose );
		LUA->SetField( -2, "GetTranspose" );

		LUA->PushCFunction( lMatrixGetForward );
		LUA->SetField( -2, "GetForward" );

		LUA->PushCFunction( lMatrixGetLeft );
		LUA->SetField( -2, "GetLeft" );

		LUA->PushCFunction( lMatrixGetUp );
		LUA->SetField( -2, "GetUp" );

		LUA->PushCFunction( lMatrixSetForward );
		LUA->SetField( -2, "SetForward" );

		LUA->PushCFunction( lMatrixSetLeft );
		LUA->SetField( -2, "SetLeft" );

		LUA->PushCFunction( lMatrixSetUp );
		LUA->SetField( -2, "SetUp" );

		LUA->PushCFunction( lMatrixSet );
		LUA->SetField( -2, "Set" );

		LUA->PushCFunction( lMatrixCopy );
		LUA->SetField( -2, "Copy" );

		LUA->PushCFunction( lMatrixSetField );
		LUA->SetField( -2, "SetField" );

		LUA->PushCFunction( lMatrixGetField );
		LUA->SetField( -2, "GetField" );

		LUA->PushCFunction( lMatrixToTable );
		LUA->SetField( -2, "ToTable" );

		LUA->PushCFunction( lMatrixFromTable );
		LUA->SetField( -2, "FromTable" );

	LUA->Pop( );

	return 0;
}

GMOD_MODULE_CLOSE( )
{
	return 0;
}