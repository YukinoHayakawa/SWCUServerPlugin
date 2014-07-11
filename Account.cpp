#include <sampgdk/a_players.h>
#include <sstream>

#include "Account.h"

Account::Account(const AccountInfo& ainfo, DataSource& db) : info(ainfo), datasrc(db)
{
	datasrc.loadAccount(info);
}

std::string Account::getLogName() const
{
	return info.logname;
}

std::string Account::getNickname() const
{
	return info.nickname;
}

std::string Account::getUserID() const
{
	return info.userid;
}

bool Account::setLogName(const std::string& name)
{
	std::string oldname = info.logname;

	int canchange = SetPlayerName(info.ingameid, name.c_str());

	switch (canchange)
	{
	case 1: // succeeded	
	{
		bool suc = datasrc.changeAccountLogName(info, name);
		if (!suc)
		{
			SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] ���ݿⷵ�ظ��ĵ�¼����ʧ��. ����ϵ����Ա.");
			SetPlayerName(info.ingameid, oldname.c_str());
			return false;
		}
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] ��¼�����ĳɹ�.");
		return true;
	}
	case 0: // same
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] Ҫ����ĵ�����������ʹ�õ���ͬ. δ���и���.");
		return false;
	}
	case -1: // invalid
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] Ҫ����ĵ����Ʋ�����Ҫ��. ������3-24���ַ����� 0-9 a-z A-Z [] () $ @ . _  = ��ɵ���Ч����.");
		return false;
	}
	}
	std::cout << "[Account] Warning: unexpected code reached in setLogName.\n";
	return false;
}

bool Account::setNickname(const std::string& name)
{
	if (datasrc.changeAccountNickName(info, name))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] �ǳƸ��ĳɹ�.");
		return true;
	}
	else
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] �ǳƸ���ʧ��. ����ϵ����Ա.");
		return false;
	}
}

int Account::getMoney() const
{
	return info.money;
}

void Account::increaseMoney(int amount, const std::string& reason)
{
	if (!datasrc.increaseAccountMoney(info, amount, reason))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] �˵�����ʧ��. ����ϵ����Ա.");
		std::cout << "[Account] Warning: increaseAccountMoney Failed.\n";
	}
}

bool Account::isRegistered() const
{
	return info.registered;
}

bool Account::isLoggedIn() const
{
	return info.loggedin;
}

bool Account::create(const std::string& rawpw)
{
	if (datasrc.createAccount(info) && datasrc.changePassword(info, rawpw))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] ע��ɹ�.");
		return true;
	}
	else
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] ע��ʧ��. ����ϵ����Ա.");
		std::cout << "[Account] Warning: create Failed.\n";
		return false;
	}
}

bool Account::auth(const std::string& rawpw)
{
	if(datasrc.authAccount(info, rawpw))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] ��¼�ɹ�.");
		return true;
	}
	else
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] ��¼ʧ��. �������������.");
		return false;
	}
}

bool Account::changePassword(const std::string& newrawpw)
{
	if (datasrc.changePassword(info, newrawpw))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] ������ĳɹ�.");
		return true;
	}
	else
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] �������ʧ��. ����ϵ����Ա.");
		std::cout << "[Account] Warning: changePassword Failed.\n";
		return false;
	}
}

int Account::getAdminLevel() const
{
	return info.adminlevel;
}

bool Account::setAdminLevel(AccountInfo& operatorAccount, int level)
{
	if (datasrc.changeAccountAdminLevel(info, operatorAccount, level))
	{
		std::stringstream str;
		str << "[Account] " << info.nickname << "(ID: " << info.userid << ") �Ĺ���Ա�ȼ�������Ϊ " << level << ".";
		SendClientMessageToAll(0xFFFFFFFF, str.str().c_str());
		return true;
	}
	else
	{
		std::cout << "[Account] Warning: changeAccountAdminLevel Failed.\n";
		return false;
	}
}

int Account::getInGameID() const
{
	return info.ingameid;
}

uint64_t Account::getSessionID() const
{
	return info.session;
}