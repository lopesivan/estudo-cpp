#include <ngspice/sharedspice.h>

#include <lua.hpp>

#include <cmath>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

static std::vector<double> g_time;

// ------------------------------------------------------------
// ngspice
// ------------------------------------------------------------

int send_char(char*, int, void*)
{
    return 0;
}

int controlled_exit(int status, bool, bool, int, void*)
{
    if(status != 0)
        std::cerr << "ngspice terminou com status "
                  << status << '\n';

    return 0;
}

void ng_command(const std::string& command)
{
    std::vector<char> buffer(
        command.begin(),
        command.end()
    );

    buffer.push_back('\0');

    if(ngSpice_Command(buffer.data()) != 0)
        throw std::runtime_error(
            "Erro ngspice: " + command
        );
}

std::vector<double> get_vector(const std::string& name)
{
    std::vector<char> buffer(
        name.begin(),
        name.end()
    );

    buffer.push_back('\0');

    pvector_info info =
        ngGet_Vec_Info(buffer.data());

    if(!info || !info->v_realdata)
        throw std::runtime_error(
            "Vetor nao encontrado: " + name
        );

    return {
        info->v_realdata,
        info->v_realdata + info->v_length
    };
}

// ------------------------------------------------------------
// RMS usando integração no tempo
// ------------------------------------------------------------

double rms(
    std::span<const double> time,
    std::span<const double> signal)
{
    if(time.size() != signal.size())
        throw std::runtime_error(
            "Vetores incompatíveis"
        );

    double integral = 0.0;

    for(std::size_t i = 1; i < signal.size(); ++i)
    {
        const double dt =
            time[i] - time[i - 1];

        const double x0 =
            signal[i - 1];

        const double x1 =
            signal[i];

        integral +=
            dt / 3.0 *
            (
                x0 * x0 +
                x0 * x1 +
                x1 * x1
            );
    }

    const double duration =
        time.back() - time.front();

    return std::sqrt(
        integral / duration
    );
}

// ------------------------------------------------------------
// Função visível para Lua:
//
// rms("in")
// ------------------------------------------------------------

int lua_rms(lua_State* L)
{
    try
    {
        const char* name =
            luaL_checkstring(L, 1);

        const auto signal =
            get_vector(name);

        const double value =
            rms(g_time, signal);

        lua_pushnumber(
            L,
            value
        );

        return 1;
    }
    catch(const std::exception& e)
    {
        return luaL_error(
            L,
            "%s",
            e.what()
        );
    }
}

// ------------------------------------------------------------

int main()
{
    try
    {
        // ngspice
        if(ngSpice_Init(
               send_char,
               nullptr,
               controlled_exit,
               nullptr,
               nullptr,
               nullptr,
               nullptr) != 0)
        {
            throw std::runtime_error(
                "Falha inicializando ngspice"
            );
        }

        ng_command(
            "source circuito.sp"
        );

        ng_command(
            "run"
        );

        g_time =
            get_vector("time");

        // Lua
        lua_State* L =
            luaL_newstate();

        if(!L)
            throw std::runtime_error(
                "Falha criando Lua"
            );

        luaL_openlibs(L);

        // registra:
        //
        // rms(...)
        //
        lua_register(
            L,
            "rms",
            lua_rms
        );

        if(luaL_dofile(
               L,
               "analise.lua") != LUA_OK)
        {
            std::cerr
                << lua_tostring(L, -1)
                << '\n';

            lua_close(L);

            return 1;
        }

        lua_close(L);

        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr
            << "Erro: "
            << e.what()
            << '\n';

        return 1;
    }
}
