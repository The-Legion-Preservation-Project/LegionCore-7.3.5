/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "PreparedStatement.h"
#include "Errors.h"
#include "MySQLConnection.h"
#include "MySQLPreparedStatement.h"
#include "QueryResult.h"
#include "Log.h"
#include "MySQLWorkaround.h"
#include <mysql.h>
#include <sstream>

PreparedStatement::PreparedStatement(uint32 index) :
m_stmt(NULL),
m_index(index) { }

PreparedStatement::~PreparedStatement() { }

void PreparedStatement::BindParameters()
{
    ASSERT(m_stmt);

    uint8 i = 0;
    for (; i < statement_data.size(); i++)
    {
        switch (statement_data[i].type)
        {
            case TYPE_BOOL:
                m_stmt->setBool(i, statement_data[i].data.boolean);
                break;
            case TYPE_UI8:
                m_stmt->setUInt8(i, statement_data[i].data.ui8);
                break;
            case TYPE_UI16:
                m_stmt->setUInt16(i, statement_data[i].data.ui16);
                break;
            case TYPE_UI32:
                m_stmt->setUInt32(i, statement_data[i].data.ui32);
                break;
            case TYPE_I8:
                m_stmt->setInt8(i, statement_data[i].data.i8);
                break;
            case TYPE_I16:
                m_stmt->setInt16(i, statement_data[i].data.i16);
                break;
            case TYPE_I32:
                m_stmt->setInt32(i, statement_data[i].data.i32);
                break;
            case TYPE_UI64:
                m_stmt->setUInt64(i, statement_data[i].data.ui64);
                break;
            case TYPE_I64:
                m_stmt->setInt64(i, statement_data[i].data.i64);
                break;
            case TYPE_FLOAT:
                m_stmt->setFloat(i, statement_data[i].data.f);
                break;
            case TYPE_DOUBLE:
                m_stmt->setDouble(i, statement_data[i].data.d);
                break;
            case TYPE_STRING:
                m_stmt->setBinary(i, statement_data[i].binary, true);
                break;
            case TYPE_BINARY:
                m_stmt->setBinary(i, statement_data[i].binary, false);
                break;
            case TYPE_NULL:
                m_stmt->setNull(i);
                break;
        }
    }
    #ifdef _DEBUG
    if (i < m_stmt->m_paramCount)
        TC_LOG_WARN("sql.sql", "[WARNING]: BindParameters() for statement %u did not bind all allocated parameters", m_index);
    #endif
}

//- Bind to buffer
void PreparedStatement::setBool(const uint8 index, const bool value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.boolean = value;
    statement_data[index].type = TYPE_BOOL;
}

void PreparedStatement::setUInt8(const uint8 index, const uint8 value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.ui8 = value;
    statement_data[index].type = TYPE_UI8;
}

void PreparedStatement::setUInt16(const uint8 index, const uint16 value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.ui16 = value;
    statement_data[index].type = TYPE_UI16;
}

void PreparedStatement::setUInt32(const uint8 index, const uint32 value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.ui32 = value;
    statement_data[index].type = TYPE_UI32;
}

void PreparedStatement::setUInt64(const uint8 index, const uint64 value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.ui64 = value;
    statement_data[index].type = TYPE_UI64;
}

void PreparedStatement::setInt8(const uint8 index, const int8 value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.i8 = value;
    statement_data[index].type = TYPE_I8;
}

void PreparedStatement::setInt16(const uint8 index, const int16 value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.i16 = value;
    statement_data[index].type = TYPE_I16;
}

void PreparedStatement::setInt32(const uint8 index, const int32 value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.i32 = value;
    statement_data[index].type = TYPE_I32;
}

void PreparedStatement::setInt64(const uint8 index, const int64 value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.i64 = value;
    statement_data[index].type = TYPE_I64;
}

void PreparedStatement::setFloat(const uint8 index, const float value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.f = value;
    statement_data[index].type = TYPE_FLOAT;
}

void PreparedStatement::setDouble(const uint8 index, const double value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].data.d = value;
    statement_data[index].type = TYPE_DOUBLE;
}

void PreparedStatement::setString(const uint8 index, const std::string& value)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].binary.resize(value.length() + 1);
    memcpy(statement_data[index].binary.data(), value.c_str(), value.length() + 1);
    statement_data[index].type = TYPE_STRING;
}

void PreparedStatement::setBinary(const uint8 index, const std::vector<uint8>& value)
{
    if (index >= statement_data.size())
        statement_data.resize(index + 1);

    statement_data[index].binary = value;
    statement_data[index].type = TYPE_BINARY;
}

void PreparedStatement::setNull(const uint8 index)
{
    if (index >= statement_data.size())
        statement_data.resize(index+1);

    statement_data[index].type = TYPE_NULL;
}

//- Execution
PreparedStatementTask::PreparedStatementTask(PreparedStatement* stmt, bool async) :
m_stmt(stmt), m_result(nullptr)
{
    m_has_result = async; // If it's async, then there's a result
    if (async)
        m_result = new PreparedQueryResultPromise();
}

PreparedStatementTask::~PreparedStatementTask()
{
    delete m_stmt;
    if (m_has_result && m_result != nullptr)
        delete m_result;
}

bool PreparedStatementTask::Execute()
{
    if (m_has_result)
    {
        PreparedResultSet* result = m_conn->Query(m_stmt);
        if (!result || !result->GetRowCount())
        {
            delete result;
            m_result->set_value(PreparedQueryResult(NULL));
            return false;
        }
        m_result->set_value(PreparedQueryResult(result));
        return true;
    }

    return m_conn->Execute(m_stmt);
}
