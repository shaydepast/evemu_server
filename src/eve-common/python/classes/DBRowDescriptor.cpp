/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2008 The EVEmu Team
    For the latest information visit http://evemu.mmoforge.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:     Bloody.Rabbit
*/

#include "EVECommonPCH.h"

#include "python/classes/DBRowDescriptor.h"
#include "utils/EVEUtils.h"

DBRowDescriptor::DBRowDescriptor()
: PyObjectEx_Type1( new PyToken( "blue.DBRowDescriptor" ), _CreateArgs(), NULL )
{
}

DBRowDescriptor::DBRowDescriptor( const DBQueryResult& res )
: PyObjectEx_Type1( new PyToken( "blue.DBRowDescriptor" ), _CreateArgs(), NULL )
{
	uint32 cc = res.ColumnCount();

	for( uint32 i = 0; i < cc; i++ )
		AddColumn( res.ColumnName( i ), res.ColumnType( i ) );
}

DBRowDescriptor::DBRowDescriptor( const DBResultRow& row )
: PyObjectEx_Type1( new PyToken( "blue.DBRowDescriptor" ), _CreateArgs(), NULL )
{
	uint32 cc = row.ColumnCount();

	for( uint32 i = 0; i < cc; i++ )
		AddColumn( row.ColumnName( i ), row.ColumnType( i ) );
}

uint32 DBRowDescriptor::ColumnCount() const
{
	return _GetColumnList()->size();
}

PyString* DBRowDescriptor::GetColumnName( uint32 index ) const
{
	return _GetColumn( index )->GetItem( 0 )->AsString();
}

DBTYPE DBRowDescriptor::GetColumnType( uint32 index ) const
{
	return (DBTYPE)_GetColumn( index )->GetItem( 1 )->AsInt()->value();
}

uint32 DBRowDescriptor::FindColumn( const char* name ) const
{
	uint32 cc = ColumnCount();
    PyString* stringName = new PyString( name );
    
	for( uint32 i = 0; i < cc; i++ )
    {
		if( stringName->hash() == GetColumnName( i )->hash() )
        {
            PyDecRef( stringName );
			return i;
        }
    }

    PyDecRef( stringName );
	return cc;
}

bool DBRowDescriptor::VerifyValue( uint32 index, PyRep* value )
{
    if( index >= ColumnCount() )
        return false;

    if( NULL != value )
    {
        if( !DBTYPE_IsCompatible( GetColumnType( index ), value ) )
            return false;
    }

    return true;
}

void DBRowDescriptor::AddColumn( const char* name, DBTYPE type )
{
	PyTuple* col = new PyTuple( 2 );

	col->SetItem( 0, new PyString( name ) );
	col->SetItem( 1, new PyInt( type ) );

	_GetColumnList()->items.push_back( col );
}

PyTuple* DBRowDescriptor::_GetColumnList() const
{
	return GetArgs()->GetItem( 0 )->AsTuple();
}

PyTuple* DBRowDescriptor::_GetColumn( size_t index ) const
{
	return _GetColumnList()->GetItem( index )->AsTuple();
}

PyTuple* DBRowDescriptor::_CreateArgs()
{
	PyTuple* columnList = new PyTuple( 0 );

	PyTuple* args = new PyTuple( 1 );
	args->SetItem( 0, columnList );

	return args;
}