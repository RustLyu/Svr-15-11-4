#include <iostream>
#include <string>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zTCPTaskPool.h"
#include "zNetService.h"
#include "zDBConnPool.h"
#include "zMisc.h"
#include "zArg.h"
#include "zConfile.h"
#include "zBase64.h"
#include "InfoTask.h"
#include "InfoServer.h"
#include "InfoCommand.h"


/**
 * \brief 鍛戒护琛屽弬鏁?
 */
static struct argp_option dbaccess_options[] =
{
	{"daemon",		  	'd',	0,				0,	"Run service as daemon",	0},
	{"log",				'l',	"level",		0,	"Log level",					0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",				0},
	{0,					0,		0,				0,	0,									0}
};


/**
 * \brief 鍛戒护琛屽弬鏁拌В鏋愬櫒
 * \param key 鍙傛暟缂╁啓
 * \param arg 鍙傛暟鍊?
 * \param state 鍙傛暟鐘舵€?
 * \return 杩斿洖閿欒浠ｇ爜
 */
static error_t dbaccess_parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'd':
			{
				Zebra::global["daemon"] = "true";
			}
			break;
		case 'l':
			{
				Zebra::global["log"]=arg;
			}
			break;
		case 'f':
			{
				Zebra::global["logfilename"]=arg;
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}


/**
 * \brief 绠€鐭弿杩颁俊鎭?
 */
static char dbaccess_doc[] = "\nInfoServer\n" "\t淇℃伅闆嗕腑鏈嶅姟鍣ㄣ€?;

/**
 * \brief 绋嬪簭鐨勭増鏈俊鎭?
 */
const char *argp_program_version = "Program version :\t" VERSION_STRING\
									"\nBuild version   :\t" _S(BUILD_STRING);


/**
 * \brief 涓荤▼搴忓叆鍙?
 * \param argc 鍙傛暟涓暟
 * \param argv 鍙傛暟鍒楄〃
 * \return 杩愯缁撴灉
 */
int main(int argc, char **argv)
{
	Zebra::logger=new zLogger("InfoServer");

	//璁剧疆缂虹渷鍙傛暟
	Zebra::global["bindport"] = "9903";
	Zebra::global["logfilename"] = "/tmp/infoserver.log";
	Zebra::global["dbCount"] = "16";
	Zebra::global["tableCount"] = "16";

	//瑙ｆ瀽閰嶇疆鏂囦欢鍙傛暟
	InfoConfile sc;
	if (!sc.parse("InfoServer"))
		return EXIT_FAILURE;

	//瑙ｆ瀽鍛戒护琛屽弬鏁?
	zArg::getArg()->add(dbaccess_options, dbaccess_parse_opt, 0, dbaccess_doc);
	zArg::getArg()->parse(argc, argv);

	//璁剧疆鏃ュ織绾у埆
	Zebra::logger->setLevel(Zebra::global["log"]);
	//璁剧疆鍐欐湰鍦版棩蹇楁枃浠?
	if ("" != Zebra::global["logfilename"])
		Zebra::logger->addLocalFileLog(Zebra::global["logfilename"]);

	//鏄惁浠ュ悗鍙拌繘绋嬬殑鏂瑰紡杩愯
	if ("true" == Zebra::global["daemon"]) {
		Zebra::logger->info("Program will be run as a daemon");
		Zebra::logger->removeConsoleLog();
		daemon(1, 1);
	}

	InfoService::getInstance().main();
	InfoService::delInstance();
	
	return EXIT_FAILURE;
}
