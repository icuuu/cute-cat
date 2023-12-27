#include "program.h"

#include <iostream>
#include <fstream>
#include <unordered_map>

#include <dpp/dpp.h>

// Command headers
#include "slashcmd/base.h"
#include "slashcmd/avatar.h"
#include "slashcmd/chess.h"
#include "slashcmd/echo.h"
// #include "slashcmd/cat.h"
#include "slashcmd/ping.h"
#include "slashcmd/uptime.h"
#include "msgcmd/base.h"
#include "msgcmd/quote.h"
#include "usercmd/base.h"
#include "usercmd/avatar.h"

Program::Program(const char* configPath)
{
	std::ifstream file(configPath);
	if (file.fail()) throw;
	this->context.config = nlohmann::json::parse(file);
	file.close();
}

void Program::run()
{
	std::unordered_map<std::string, SlashCommand::Base*> slashCommands = 
	{
		{ "avatar", new SlashCommand::Avatar },
		{ "chess", new SlashCommand::Chess },
		{ "echo", new SlashCommand::Echo },
		{ "ping", new SlashCommand::Ping },
		{ "uptime", new SlashCommand::Uptime }
		// { "cat", new SlashCommand::Cat }
	};

	std::unordered_map<std::string, MessageCommand::Base*> messageCommands =
	{
		{ "quote", new MessageCommand::Quote }
	};

	std::unordered_map<std::string, UserCommand::Base*> userCommands =
	{
		{ "avatar", new UserCommand::Avatar }
	};

	dpp::cluster bot(this->context.config.at("token"), dpp::i_all_intents);

	// bot.on_log(dpp::utility::cout_logger());

	bot.on_message_create([this, &bot](const dpp::message_create_t& event)
	{
		bool isGuildPriveleged = true;
		/*
		std::vector<uint64_t> privelegedGuilds = this->context.config.at("priveleged_guilds").get<std::vector<uint64_t>>();
		for (const auto& id : privelegedGuilds)
		{
			if (id == event.msg.guild_id)
				isGuildPriveleged == true;
		}
		*/

		if (isGuildPriveleged)
		{
			if (event.msg.content.contains("edward"))
				bot.message_add_reaction(event.msg, "edward:1042165132555460639");
		}
	});

	bot.on_guild_member_add([this, &bot](const dpp::guild_member_add_t& event)
	{
		if (event.adding_guild->id == 746067864481431562)
		{
			dpp::embed embed;
			embed.set_color(this->context.config.at("embed_colors").at("guild_member_add"));
			embed.set_description("**" + event.added.get_user()->username + "** has joined");
			embed.set_thumbnail(event.added.get_user()->get_avatar_url());
			bot.message_create(dpp::message(746067864481431565, embed));
		}
	});

	bot.on_guild_member_remove([this, &bot](const dpp::guild_member_remove_t& event)
	{
		if (event.removing_guild->id == 746067864481431562)
		{
			dpp::embed embed;
			embed.set_color(this->context.config.at("embed_colors").at("guild_member_remove"));
			embed.set_description("**" + event.removed.username + "** has left");
			embed.set_thumbnail(event.removed.get_avatar_url());
			bot.message_create(dpp::message(746067864481431565, embed));
		}
	});

	bot.on_slashcommand([this, &bot, &slashCommands](const dpp::slashcommand_t& event)
	{
		slashCommands[event.command.get_command_name()]->execute(this->context, bot, event);
	});

	bot.on_message_context_menu([this, &bot, &messageCommands](const dpp::message_context_menu_t& event)
	{
		messageCommands[event.command.get_command_name()]->execute(this->context, bot, event);	
	});

	bot.on_user_context_menu([this, &bot, &userCommands](const dpp::user_context_menu_t& event)
	{
		userCommands[event.command.get_command_name()]->execute(this->context, bot, event);
	});

	bot.on_ready([this, &bot, &slashCommands, &messageCommands, &userCommands](const dpp::ready_t& event)
	{
		printf("press 'y' to delete all commands, press anything else to disregard\n");
		if (std::cin.get() == 'y') bot.global_bulk_command_delete_sync();
		
		printf("Server, ID\n");
		for (const auto& [snowflake, guild] : bot.current_user_get_guilds_sync())
		{
			printf("%s, %s\n", guild.name.c_str(), snowflake.str().c_str());
		}

		bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, this->context.config.at("status")));

		if (dpp::run_once<struct register_bot_commands>())
        	{
			for (auto& [name, cmd] : slashCommands)
                		bot.global_command_create(cmd->apiObj(bot, name));

			for (auto& [name, cmd] : messageCommands)
				bot.global_command_create(cmd->apiObj(bot, name));
        	
			for (auto& [name, cmd] : userCommands)
				bot.global_command_create(cmd->apiObj(bot, name));
		}
	});

	bot.start(dpp::st_wait);
}
