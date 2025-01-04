#include "lua.hpp"

#define LED_PIN 2

lua_State *L;

static int l_digitalWrite(lua_State *L)
{
    int value = lua_tointeger(L, 1);
    digitalWrite(LED_PIN, value);
    return 0;
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    L = luaL_newstate();
    if (L == NULL)
    {
        Serial.println("Failed to create Lua state");
        return;
    }
    luaL_openlibs(L);
    lua_register(L, "digitalWrite", l_digitalWrite);

    const char *blinkScript = R"(
        print('Lua script starting...')
        function setup()
            print('Lua setup running')
            period = 1/5  -- 5 Hz
            print('Period set to: ' .. period)
        end

        lastToggle = 0
        state = 0

        function loop(now)
            if now - lastToggle >= period then
                state = 1 - state
                digitalWrite(state)
                lastToggle = now
                print('Toggle at time: ' .. now .. ' state: ' .. state)
            end
        end

        print('Running setup...')
        setup()
        print('Setup complete')
    )";

    if (luaL_dostring(L, blinkScript) != LUA_OK)
    {
        Serial.println("Lua error:");
        Serial.println(lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void loop()
{
    lua_getglobal(L, "loop");
    lua_pushnumber(L, millis() / 1000.0);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
    {
        Serial.println("Lua error:");
        Serial.println(lua_tostring(L, -1));
        lua_pop(L, 1);
        delay(1000);
    }
}
