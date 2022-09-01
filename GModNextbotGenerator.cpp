#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;


void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
	size_t pos = data.find(toSearch);
	while (pos != std::string::npos)
	{
		data.replace(pos, toSearch.size(), replaceStr);
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}


int main()
{
	// Prepairing
	std::ifstream nextbotLuaFile("nextbot_example.lua");
	std::stringstream filestringstream;
	std::string line;

	while (std::getline(nextbotLuaFile, line)) {
		filestringstream << line << std::endl;
	}

	std::string luaFile = filestringstream.str();
	// Prepaired

	std::string gmodPath;
	std::string PNGPath;


	std::string nextbotName;
	std::string nextbotDisplayName;	

	std::string jumpSound;
	std::string highJumpSound;
	std::string chaseSound;

	std::vector<std::string> tauntSounds;


	// Questioning



	std::cout << "Garry's Mod Nextbot Generator" << std::endl;
	std::cout << "Made by Livelandr" << std::endl << std::endl;


	std::cout << "Input your Garry's mod folder path: ";
	std::getline(std::cin, gmodPath);

	gmodPath = gmodPath + ((gmodPath[gmodPath.size() - 1] == '/') ? "" : "/");

	if (!(fs::exists(gmodPath + "/hl2.exe"))) {
		std::cout << "Invalid path!" << std::endl;
		return 1;
	}


	std::cout << "Input nextbot entity name (Without spaces, only latinate!): ";
	std::cin >> nextbotName;

	std::cin.ignore();

	std::cout << "Input nextbot display name: ";
	std::getline(std::cin, nextbotDisplayName);

	std::cout << "Input PNG file path (Nextbot face): ";
	std::cin >> PNGPath;

	if (!(fs::exists(PNGPath))) {
		std::cout << "Invalid path!" << std::endl;
		return 1;
	}

	if (PNGPath.substr(PNGPath.find_last_of(".") + 1) != "png") {
		std::cout << "This file is not .png!" << std::endl;
		return 1;
	}

	std::cin.ignore();

	std::cout << "Input chase sound (.mp3 file): ";
	std::getline(std::cin, chaseSound);

	if (!(fs::exists(chaseSound))) {
		std::cout << "Invalid path!" << std::endl;
		return 1;
	}

	std::cout << "Input jump sound (.mp3 file): ";
	std::getline(std::cin, jumpSound);

	if (!(fs::exists(jumpSound))) {
		std::cout << "Invalid path!" << std::endl;
		return 1;
	}

	std::cout << "Input high jump sound (.mp3 file): ";
	std::getline(std::cin, highJumpSound);

	if (!(fs::exists(highJumpSound))) {
		std::cout << "Invalid path!" << std::endl;
		return 1;
	}

	int tauntSoundsCount;


	std::cout << "How much taunt sounds you want?: ";
	std::cin >> tauntSoundsCount;

	if (tauntSoundsCount < 1) {
		std::cout << "At least one taunt!" << std::endl;
		return 1;
	}

	std::cin.ignore();

	for (int i = 0; i < tauntSoundsCount; i++) {

		std::string tauntSound;

		std::cout << "Taunt sound #" << i + 1 << " path (.mp3): ";
		std::getline(std::cin, tauntSound);

		if (!(fs::exists(tauntSound))) {
			std::cout << "Invalid path!" << std::endl;
			return 1;
		}

		tauntSounds.push_back(tauntSound);

	}

	// Questioning ended!

	std::string NextbotSoundsData;

	NextbotSoundsData += R"(
	["HighJump"] = ")" + nextbotName + "/" + "highjump.mp3" + "\",";
	NextbotSoundsData += R"(
	["Jump"] = ")" + nextbotName + "/" + "jump.mp3" + "\",";
	NextbotSoundsData += R"(
	["ChaseSound"] = ")" + nextbotName + "/" + "chase.mp3" + "\",";
	NextbotSoundsData += R"(
	["tauntSounds"] = {
)";
	
	for (int i = 0; i < tauntSoundsCount; i++) {
		 
		NextbotSoundsData += " \"" +  nextbotName + "/tauntSound_" + std::to_string(i + 1) + ".mp3\",";

	}


	NextbotSoundsData += "\n}";

	// Nextbot generation


	findAndReplaceAll(luaFile, "REPLACEME_NEXTBOTNAME", nextbotName);
	findAndReplaceAll(luaFile, "REPLACEME_NEXTBOTDISPLAYNAME", nextbotDisplayName);
	findAndReplaceAll(luaFile, "REPLACEME_NEXTBOTSOUNDS", NextbotSoundsData);

	fs::current_path(gmodPath + "garrysmod/addons/");
	fs::create_directory("generated_nextbot_" + nextbotName);

	fs::current_path(gmodPath + "garrysmod/addons/generated_nextbot_" + nextbotName);

	fs::create_directories("lua/entities");
	fs::create_directories("sound/" + nextbotName);
	fs::create_directories("materials/" + nextbotName);

	std::ofstream readyFile(gmodPath + "garrysmod/addons/generated_nextbot_" + nextbotName + "/lua/entities/npc_" + nextbotName + ".lua");
	readyFile << luaFile << std::endl;
	readyFile.close();
	
	std::string soundPath = gmodPath + "garrysmod/addons/generated_nextbot_" + nextbotName + "/sound/" + nextbotName;

	fs::copy(jumpSound, soundPath + "/jump.mp3", std::filesystem::copy_options::overwrite_existing);
	fs::copy(highJumpSound, soundPath + "/highjump.mp3", std::filesystem::copy_options::overwrite_existing);
	fs::copy(chaseSound, soundPath + "/chase.mp3", std::filesystem::copy_options::overwrite_existing);


	for (int i = 0; i < tauntSoundsCount; i++) {
		fs::copy( tauntSounds[i], soundPath + "/tauntSound_" + std::to_string(i + 1) + ".mp3", std::filesystem::copy_options::overwrite_existing);
	}

	fs::copy(PNGPath, gmodPath + "garrysmod/addons/generated_nextbot_" + nextbotName + "/materials/" + nextbotName + "/npc_" + nextbotName + ".png", std::filesystem::copy_options::overwrite_existing);

}