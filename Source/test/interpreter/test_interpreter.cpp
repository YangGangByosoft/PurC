#include "purc.h"

#include <gtest/gtest.h>


static const char *calculator_1 =
    "<!DOCTYPE hvml>"
    "<hvml target=\"html\" lang=\"en\">"
    "    <head>"
    "        <title>计算器</title>"
    "        <link rel=\"stylesheet\" type=\"text/css\" href=\"calculator.css\" />"
    ""
    "        <init as=\"buttons\" uniquely>"
    "            ["
    "                { \"letters\": \"7\", \"class\": \"number\" },"
    "                { \"letters\": \"8\", \"class\": \"number\" },"
    "                { \"letters\": \"9\", \"class\": \"number\" },"
    "                { \"letters\": \"←\", \"class\": \"c_blue backspace\" },"
    "                { \"letters\": \"C\", \"class\": \"c_blue clear\" },"
    "                { \"letters\": \"4\", \"class\": \"number\" },"
    "                { \"letters\": \"5\", \"class\": \"number\" },"
    "                { \"letters\": \"6\", \"class\": \"number\" },"
    "                { \"letters\": \"×\", \"class\": \"c_blue multiplication\" },"
    "                { \"letters\": \"÷\", \"class\": \"c_blue division\" },"
    "                { \"letters\": \"1\", \"class\": \"number\" },"
    "                { \"letters\": \"2\", \"class\": \"number\" },"
    "                { \"letters\": \"3\", \"class\": \"number\" },"
    "                { \"letters\": \"+\", \"class\": \"c_blue plus\" },"
    "                { \"letters\": \"-\", \"class\": \"c_blue subtraction\" },"
    "                { \"letters\": \"0\", \"class\": \"number\" },"
    "                { \"letters\": \"00\", \"class\": \"number\" },"
    "                { \"letters\": \".\", \"class\": \"number\" },"
    "                { \"letters\": \"%\", \"class\": \"c_blue percent\" },"
    "                { \"letters\": \"=\", \"class\": \"c_yellow equal\" },"
    "            ]"
    "        </init>"
    "    </head>"
    ""
    "    <body>"
    "        <div id=\"calculator\">"
    ""
    "            <div id=\"c_title\">"
    "                <h2>计算器</h2>"
    "            </div>"
    ""
    "            <div id=\"c_text\">"
    "                <input type=\"text\" id=\"text\" value=\"0\" readonly=\"readonly\" />"
    "            </div>"
    ""
    "            <div id=\"c_value\">"
    "                <archetype name=\"button\">"
    "                    <li class=\"$?.class\">$?.letters</li>"
    "                </archetype>"
    ""
    "                <ul>"
    "                    <iterate on=\"$buttons\">"
    "                        <update on=\"$@\" to=\"append\" with=\"$button\" />"
    "                        <except type=\"NoData\" raw>"
    "                            <p>Bad data!</p>"
    "                        </except>"
    "                    </iterate>"
    "                </ul>"
    "            </div>"
    "        </div>"
    "    </body>"
    ""
    "</hvml>";

static const char *calculator_2 =
    "<!DOCTYPE hvml>"
    "<hvml target=\"html\" lang=\"en\">"
    "    <head>"
    "        <base href=\"$HVML.base(! 'https://gitlab.fmsoft.cn/hvml/hvml-docs/raw/master/samples/calculator/' )\" />"
    ""
    "        <update on=\"$T.map\" from=\"assets/{$SYSTEM.locale}.json\" to=\"merge\" />"
    ""
    "        <init as=\"buttons\" from=\"assets/buttons.json\" />"
    ""
    "        <title>$T.get('HVML Calculator')</title>"
    ""
    "        <update on=\"$TIMERS\" to=\"displace\">"
    "            ["
    "                { \"id\" : \"clock\", \"interval\" : 1000, \"active\" : \"yes\" },"
    "            ]"
    "        </update>"
    ""
    "        <link rel=\"stylesheet\" type=\"text/css\" href=\"assets/calculator.css\" />"
    "    </head>"
    ""
    "    <body>"
    "        <div id=\"calculator\">"
    ""
    "            <div id=\"c_text\">"
    "                <input type=\"text\" id=\"text\" value=\"0\" readonly=\"readonly\" />"
    "            </div>"
    ""
    "            <div id=\"c_value\">"
    "                <archetype name=\"button\">"
    "                    <li class=\"$?.class\">$?.letters</li>"
    "                </archetype>"
    ""
    "                <ul>"
    "                    <iterate on=\"$buttons\">"
    "                        <update on=\"$@\" to=\"append\" with=\"$button\" />"
    "                        <except type=\"NoData\" raw>"
    "                            <p>Bad data!</p>"
    "                        </except>"
    "                    </iterate>"
    "                </ul>"
    "            </div>"

    "            <div id=\"c_title\">"
    "                <h2 id=\"c_title\">$T.get('HVML Calculator')"
    "                    <small>$T.get('Current Time: ')<span id=\"clock\">$SYSTEM.time('%H:%M:%S')</span></small>"
    "                </h2>"
    "                <observe on=\"$TIMERS\" for=\"expired:clock\">"
    "                    <update on=\"#clock\" at=\"textContent\" with=\"$SYSTEM.time('%H:%M:%S')\" />"
    "                </observe>"
    "            </div>"
    "        </div>"
    "    </body>"
    ""
    "</hvml>";

static const char *calculator_3 =
    "<!DOCTYPE hvml SYSTEM 'v: MATH'>"
    "<hvml target=\"html\" lang=\"en\">"
    "    <head>"
    "        <base href=\"$HVML.base(! 'https://gitlab.fmsoft.cn/hvml/hvml-docs/raw/master/samples/calculator/' )\" />"
    ""
    "        <update on=\"$T.map\" from=\"assets/{$SYSTEM.locale}.json\" to=\"merge\" />"
    ""
    "        <init as=\"buttons\" from=\"assets/buttons.json\" />"
    ""
    "        <title>$T.get('HVML Calculator')</title>"
    ""
    "        <update on=\"$TIMERS\" to=\"unite\">"
    "            ["
    "                { \"id\" : \"clock\", \"interval\" : 1000, \"active\" : \"yes\" },"
    "                { \"id\" : \"input\", \"interval\" : 1500, \"active\" : \"yes\" },"
    "            ]"
    "        </update>"
    ""
    "        <link rel=\"stylesheet\" type=\"text/css\" href=\"assets/calculator.css\" />"
    "    </head>"
    ""
    "    <body>"
    "        <div id=\"calculator\">"
    ""
    "            <div id=\"c_title\">"
    "                <h2 id=\"c_title\">$T.get('HVML Calculator')"
    "                    <small>$T.get('Current Time: ')<span id=\"clock\">$SYSTEM.time('%H:%M:%S')</span></small>"
    "                </h2>"
    "                <observe on=\"$TIMERS\" for=\"expired:clock\">"
    "                    <update on=\"#clock\" at=\"textContent\" with=\"$SYSTEM.time('%H:%M:%S')\" />"
    "                </observe>"
    "            </div>"
    ""
    "            <div id=\"c_text\">"
    "                <input type=\"text\" id=\"expression\" value=\"0\" readonly=\"readonly\" />"
    "                <observe on=\"$TIMERS\" for=\"expired:input\">"
    "                    <test on=\"$buttons[$SYSTEM.random($EJSON.count($buttons))].letters\">"
    ""
    "                        <match for=\"ANY\" exclusively>"
    "                            <update on=\"$TIMERS\" to=\"overwrite\">"
    "                                { \"id\" : \"input\", \"active\" : \"no\" }"
    "                            </update>"
    "                        </match>"
    "<!--"
    "                        <match for=\"AS '='\" exclusively>"
    "                            <choose on=\"$MATH.eval($DOC.query('#expression').attr('value'))\">"
    "                                <update on=\"#expression\" at=\"attr.value\" with=\"$?\" />"
    "                                <update on=\"$TIMERS\" to=\"overwrite\">"
    "                                    { \"id\" : \"input\", \"active\" : \"no\" }"
    "                                </update>"
    "                                <catch for='*'>"
    "                                    <update on=\"#expression\" at=\"attr.value\" with=\"ERR\" />"
    "                                </catch>"
    "                            </choose>"
    "                        </match>"
    ""
    "                        <match for=\"AS 'C'\" exclusively>"
    "                            <update on=\"#expression\" at=\"attr.value\" with=\"\" />"
    "                        </match>"
    ""
    "                        <match for=\"AS '←'\" exclusively>"
    "                            <choose on=\"$DOC.query('#expression').attr.value\">"
    "                                <update on=\"#expression\" at=\"attr.value\" with=\"$STR.substr($?, 0, -1)\" />"
    "                            </choose>"
    "                        </match>"
    ""
    "                        <match>"
    "                            <update on=\"#expression\" at=\"attr.value\" with $= \"$?\" />"
    "                        </match>"
    "-->"
    "                    </test>"
    "                </observe>"
    "            </div>"
    ""
    "            <div id=\"c_value\">"
    "                <archetype name=\"button\">"
    "                    <li class=\"$?.class\">$?.letters</li>"
    "                </archetype>"
    ""
    "                <ul>"
    "                    <iterate on=\"$buttons\">"
    "                        <update on=\"$@\" to=\"append\" with=\"$button\" />"
    "                        <except type=\"NoData\" raw>"
    "                            <p>Bad data!</p>"
    "                        </except>"
    "                    </iterate>"
    "                </ul>"
    "            </div>"
    "        </div>"
    "    </body>"
    ""
    "</hvml>";

static const char *sample1 =
    "<!DOCTYPE hvml>"
    "<hvml target=\"html\">"
    "    <head>"
    "    </head>"
    ""
    "    <body>"
    "        <init as=\"global\">"
    "            { \"locale\" : \"zh_CN\" }"
    "        </init>"
    ""
    "        <archetype name=\"footer_cn\">"
    "            <p><a href=\"http://www.baidu.com\">百度</a></p>"
    "        </archetype>"
    ""
    "        <archetype name=\"footer_tw\">"
    "            <p><a href=\"http://www.bing.com\">Bing</a></p>"
    "        </archetype>"
    ""
    "        <archetype name=\"footer_def\">"
    "            <p><a href=\"http://www.google.com\">Google</a></p>"
    "        </archetype>"
    ""
    "        <footer id=\"the-footer\">"
    "            <test on=\"$global.locale\" in='#the-footer'>"
    "                <match for=\"AS 'zh_CN'\" exclusively>"
    "                    <update on=\"$@\" to=\"append\" with=\"$footer_cn\" />"
    "                </match>"
    "                <match for=\"AS 'zh_TW'\" exclusively>"
    "                    <update on=\"$@\" to=\"append\" with=\"$footer_tw\" />"
    "                </match>"
    "                <match for=\"ANY\" to=\"displace\" with=\"$footer_def\">"
    "                    <update on=\"$@\" to=\"append\" with=\"$footer_def\" />"
    "                </match>"
    "                <except type=\"NoData\" raw>"
    "                    <p>You forget to define the $global variable!</p>"
    "                </except>"
    "                <except type=\"NoSuchKey\">"
    "                    <p>Bad global data!</p>"
    "                </except>"
    "                <except type=\"IdentifierError\">"
    "                    <p>Bad archetype data!</p>"
    "                </except>"
    "            </test>"
    "        </footer>"
    "    </body>"
    "</hvml>";

TEST(interpreter, basic)
{
    (void)calculator_1;
    (void)calculator_2;
    (void)calculator_3;
    (void)sample1;

    const char *hvmls[] = {
        // "<hvml><head x=\"y\">hello<xinit a=\"b\">world<!--yes-->solid</xinit></head><body><timeout1/><timeout3/></body></hvml>",
        // "<hvml><head x=\"y\">hello<xinit a=\"b\">w<timeout3/>orld<!--yes-->solid</xinit></head><body><timeout1/></body></hvml>",
        // "<hvml><body><timeout1/><timeout9/><timeout2/></body></hvml>",
        // "<hvml><body><test a='b'>hello<!--yes--></test></body></hvml>",
        // "<hvml><body><archetype name=\"$?.button\"><li class=\"class\">letters</li></archetype></body></hvml>",
        // "<hvml><body><archetype name=\"button\"><li class=\"class\">letters</li></archetype></body></hvml>",
        // "<hvml><body><a><b><c></c></b></a></body></hvml>",
        // calculator_1,
        // calculator_2,
        calculator_3,
        // sample1,
    };

    purc_instance_extra_info info = {};
    int ret = 0;
    bool cleanup = false;

    // initial purc
    ret = purc_init ("cn.fmsoft.hybridos.test", "test_init", &info);

    ASSERT_EQ (ret, PURC_ERROR_OK);

    // get statitics information
    struct purc_variant_stat * stat = purc_variant_usage_stat ();
    ASSERT_NE(stat, nullptr);

    for (size_t i=0; i<PCA_TABLESIZE(hvmls); ++i) {
        const char *hvml = hvmls[i];
        purc_vdom_t vdom = purc_load_hvml_from_string(hvml);
        ASSERT_NE(vdom, nullptr);
    }

    purc_run(PURC_VARIANT_INVALID, NULL);

    cleanup = purc_cleanup ();
    ASSERT_EQ (cleanup, true);
}

