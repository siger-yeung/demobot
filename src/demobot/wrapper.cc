#include <extdll.h>
#include <meta_api.h>
#include <dllapi.h>
#include <sdk_util.h>
#include <h_export.h>
#include "demobot.h"
#include <vector>
#include <memory>

#include "wrapper.h"

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
globalvars_t *gpGlobals;
mutil_funcs_t *gpMetaUtilFuncs = nullptr;
globalvars_t *globals = nullptr;
enginefuncs_t meta_engfuncs = {nullptr};
enginefuncs_t g_engfuncs;

static DLL_FUNCTIONS gFunctionTable = {nullptr};

plugin_info_t Plugin_info = {
    META_INTERFACE_VERSION,
    "demobot",
    "0.1",
    "2021",
    "yscale",
    "http://www.metamod.org/",
    "STUB",
    PT_ANYTIME,
    PT_ANYPAUSE,
};

std::vector<scale::demobot> bots;

YMOD_EXPORT int GetEntityAPI2(DLL_FUNCTIONS *table, int *interfaceVersion)
{
    if (!table)
    {
        return false;
    }
    else if (*interfaceVersion != INTERFACE_VERSION)
    {
        *interfaceVersion = INTERFACE_VERSION;
        return false;
    }
    // memcpy(table, &gFunctionTable, sizeof(DLL_FUNCTIONS));
    memset(table, 0, sizeof(DLL_FUNCTIONS));
    table->pfnClientConnect = [](edict_t *entity, const char *, const char *, char *)
    {
        return 0;
    };
    table->pfnServerActivate = [](edict_t *pEdictList, int edictCount, int clientMax)
    {
        SERVER_PRINT("[DEMOBOT] SERVER ACTIVATE!!!\n");
    };
    table->pfnServerDeactivate = []()
    {
        for (auto &bot : bots)
        {
            bot.update();
            bot.think();
        }
    };
    table->pfnStartFrame = []()
    {
        if (bots.size() == 0)
        {
            scale::demobot bot("test");
            bot.load_file("cstrike/bkz_goldbhop_topoviygus_0126.30.dem");
            bots.push_back(bot);
        }
        for (auto &bot : bots)
        {
            bot.update();
            bot.think();
        }
    };
    return true;
};

YMOD_EXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine,
                                   int *interfaceVersion)
{
    if (!pengfuncsFromEngine)
    {
        return false;
    }
    else if (*interfaceVersion != ENGINE_INTERFACE_VERSION)
    {
        *interfaceVersion = ENGINE_INTERFACE_VERSION;
        return false;
    }
    memcpy(pengfuncsFromEngine, &meta_engfuncs, sizeof(enginefuncs_t));
    return true;
}

void GiveFnptrsToDll(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals)
{
    printf("[DEMOBOT] give fnptrs to dll\n");
    memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
    gpGlobals = globals = pGlobals;
}

// metamod wrapper
YMOD_EXPORT int Meta_Query(char *interfaceVersion,
                           plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs)
{
    *pPlugInfo = &Plugin_info;
    gpMetaUtilFuncs = pMetaUtilFuncs;
    return true;
};

YMOD_EXPORT int Meta_Attach(PLUG_LOADTIME now,
                            META_FUNCTIONS *pFunctionTable,
                            meta_globals_t *pMGlobals,
                            gamedll_funcs_t *pGamedllFuncs)
{
    printf("[DEMOBOT] META ATTACHED!\n");
    if (!pMGlobals)
    {
        LOG_ERROR(PLID, "Meta_Attach called with null pMGlobals");
        return false;
    }
    gpMetaGlobals = pMGlobals;
    if (!pFunctionTable)
    {
        LOG_ERROR(PLID, "Meta_Attach called with null pFunctionTable");
        return false;
    }
    static META_FUNCTIONS gMetaFunctionTable = {
        nullptr,
        nullptr,
        GetEntityAPI2,
        nullptr,
        nullptr,
        nullptr,
        GetEngineFunctions,
        nullptr};

    memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
    gpGamedllFuncs = pGamedllFuncs;
    return true;
}

YMOD_EXPORT int Meta_Detach(PLUG_LOADTIME now,
                            PL_UNLOAD_REASON reason)
{
    return true;
}