#include <vector>
#include <utility>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>

#include "GameServer.h"
#include "AdminCommands.h"
#include "StringUtility.h"
#include "Teleport.h"

#define ADMIN_CMD_BEGIN_ATR(name, cnname, reqlv, reg) \
class __AdminCmd##name : public AdminCommandBase{ public: __AdminCmd##name() : AdminCommandBase(cnname, reqlv, ADMIN_TARGET_REASON, reg) {} virtual void executeATR(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target, const std::string& reason)

#define ADMIN_CMD_BEGIN_AT(name, cnname, reqlv, reg) \
class __AdminCmd##name : public AdminCommandBase{ public: __AdminCmd##name() : AdminCommandBase(cnname, reqlv, ADMIN_TARGET, reg) {} virtual void executeAT(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target)

#define ADMIN_CMD_BEGIN_AR(name, cnname, reqlv, reg) \
class __AdminCmd##name : public AdminCommandBase{ public: __AdminCmd##name() : AdminCommandBase(cnname, reqlv, ADMIN_REASON, reg) {} virtual void executeAR(const std::shared_ptr<Account>& admin, const std::string& reason)

#define ADMIN_CMD_BEGIN_A(name, cnname, reqlv, reg) \
class __AdminCmd##name : public AdminCommandBase{ public: __AdminCmd##name() : AdminCommandBase(cnname, reqlv, ADMIN, reg) {} virtual void executeA(const std::shared_ptr<Account>& admin)

#define ADMIN_CMD_END(name) } __AdminCmdIns##name;

#define ADMIN_LOG(admin, target, reason) \
	GameServer::getInstance().datasource.adminOperationLog(admin->getUserID(), target->getUserID(), __FUNCTION__, reason)

std::vector<AdminCommandBase*> g_admin_cmd_reg;

using namespace std::placeholders;

ADMIN_CMD_BEGIN_AT(PM, "密语", 0, g_admin_cmd_reg)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(admin, "发送密语", "请输入消息", "发送", "取消", false,
	[admin, target](const std::string& msg) {
		SendClientMessage(target->getInGameID(), 0xFFFFFFFF, STR("[Message] 来自 " << admin->getLogName() << UID(admin) <<
			" 的消息: " << msg).c_str());
		ADMIN_LOG(admin, target, msg);
	});
}
ADMIN_CMD_END(PM)

ADMIN_CMD_BEGIN_AT(Goto, "传送过去", 1, g_admin_cmd_reg)
{
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 传送到了 " << target->getLogName() << UID(target) << " 身边.").c_str());
	teleportToPlayer(admin->getInGameID(), target->getInGameID());
	ADMIN_LOG(admin, target, "");
}
ADMIN_CMD_END(Goto)

ADMIN_CMD_BEGIN_AT(Get, "拉过来", 1, g_admin_cmd_reg)
{
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 将 " << target->getLogName() << UID(target) << " 传送到了自己身边.").c_str());
	teleportToPlayer(target->getInGameID(), admin->getInGameID());
	ADMIN_LOG(admin, target, "");
}
ADMIN_CMD_END(Get)

ADMIN_CMD_BEGIN_ATR(Jail, "送进监狱", 2, g_admin_cmd_reg)
{
	SendClientMessage(admin->getInGameID(), 0xFFFFFFFF, "[Admin] 监狱功能尚未完成.");
	ADMIN_LOG(admin, target, "try to use jail function(unfinished)");
}
ADMIN_CMD_END(Jail)

ADMIN_CMD_BEGIN_ATR(Freeze, "冻结", 2, g_admin_cmd_reg)
{
	target->setFreezed(true);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 冻结了 " << target->getLogName() << UID(target) << ". 原因: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(Freeze)

ADMIN_CMD_BEGIN_AT(Unfreeze, "解冻", 1, g_admin_cmd_reg)
{
	target->setFreezed(false);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 解冻了 " << target->getLogName() << UID(target) << ".").c_str());
	ADMIN_LOG(admin, target, "");
}
ADMIN_CMD_END(Unfreeze)

ADMIN_CMD_BEGIN_ATR(Mute, "禁言", 2, g_admin_cmd_reg)
{
	target->setMuted(true);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 将 " << target->getLogName() << UID(target) << " 禁言. 原因: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(Mute)

ADMIN_CMD_BEGIN_AT(Unmute, "解禁言", 1, g_admin_cmd_reg)
{
	target->setMuted(false);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 取消了对 " << target->getLogName() << UID(target) << " 的禁言.").c_str());
	ADMIN_LOG(admin, target, "");
}
ADMIN_CMD_END(Unmute)

ADMIN_CMD_BEGIN_ATR(Kill, "杀死", 2, g_admin_cmd_reg)
{
	SetPlayerHealth(target->getInGameID(), 0.0f);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 重置了 " << target->getLogName() << UID(target) << " 的生命值. 原因: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(Kill)

ADMIN_CMD_BEGIN_ATR(Warn, "警告", 1, g_admin_cmd_reg)
{
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 警告 " << target->getLogName() << UID(target) << ": " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(Warn)

ADMIN_CMD_BEGIN_ATR(KickPlayer, "踢出服务器", 2, g_admin_cmd_reg)
{
	Kick(target->getInGameID());
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 将 " << target->getLogName() << UID(target) << " 踢出服务器. 原因: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(KickPlayer)

// TO-DO: should be server cmd
ADMIN_CMD_BEGIN_AR(Announce, "宣布", 2, g_admin_cmd_reg)
{
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 宣布: " << reason).c_str());
	ADMIN_LOG(admin, GameServer::getInstance().accountmanager.findAccount(-1), reason);
}
ADMIN_CMD_END(Announce)

ADMIN_CMD_BEGIN_ATR(BanPlayer, "封禁", 3, g_admin_cmd_reg)
{
	target->setDisabled(true);
	char ip[16];
	GetPlayerIp(target->getInGameID(), ip, 16);
	char gpcidest[25];
	gpci(target->getInGameID(), gpcidest, 25);
	GameServer::getInstance().datasource.addBanRecord(admin->_getAccountInfo(), target->getLogName(), ip, gpcidest);
	Kick(target->getInGameID());
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 封禁了 " << target->getLogName() << UID(target) << ". 原因: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(BanPlayer)

// TO-DO: should be server cmd
ADMIN_CMD_BEGIN_AR(LockServer, "锁定服务器", 3, g_admin_cmd_reg)
{
	SendClientMessage(admin->getInGameID(), 0xFFFFFFFF, "[Admin] 锁定服务器功能尚未完成.");
	ADMIN_LOG(admin, GameServer::getInstance().accountmanager.findAccount(-1), reason);
}
ADMIN_CMD_END(LockServer)

ADMIN_CMD_BEGIN_ATR(SetLevel, "更改管理员等级", 3, g_admin_cmd_reg)
{
	int level = atoi(reason.c_str());
	target->setAdminLevel(admin, level);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] 管理员 " << admin->getLogName() << UID(admin) <<
		" 将 " << target->getLogName() << UID(target) << " 的管理员等级设置为 " << level << ".").c_str());
	ADMIN_LOG(admin, target, STR(level));
}
ADMIN_CMD_END(SetLevel)

void showAdminDialog(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target)
{
	DialogItemList list;

	for (auto i : g_admin_cmd_reg)
	{
		if (i->requiredlevel > admin->getAdminLevel())
		{
			continue;
		}

		switch (i->type)
		{
		case ADMIN:
			list.append(i->name, std::bind(&AdminCommandBase::executeA, i, admin));
			break;
		case ADMIN_TARGET:
			list.append(i->name, std::bind(&AdminCommandBase::executeAT, i, admin, target));
			break;
		case ADMIN_REASON:
			list.append(i->name, std::bind(&showAdminInputDialog, admin,
				[admin, i](const std::string& reason) { i->executeAR(admin, reason); }));
			break;
		case ADMIN_TARGET_REASON:
			list.append(i->name, std::bind(&showAdminInputDialog, admin,
				[admin, target, i](const std::string& reason) { i->executeATR(admin, target, reason); }));
			break;
		default:
			std::cout << "[AdminCommands] Warning: Unexpected command type.\n";
		}
	}

	GameServer::getInstance().dialogmanager.displayListDialog(admin, "玩家管理", list, "确定", "取消");
}

void showAdminInputDialog(const std::shared_ptr<Account>& player, const std::function<void(const std::string&)>& callback)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "原因", "输入原因", "确定", "取消", false, callback);
}
