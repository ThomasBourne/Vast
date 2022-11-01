#include "savestate.hpp"
#include <iostream>
#include <fstream>

namespace ss {
	template<class Func> void RecursiveJSONTraverse(json&, SaveState&, bool&, Func, std::string = "/");

	SaveState::SaveState(std::string file) {
		this->file = file;
		std::ifstream fstream(file);
		fstream >> this->raw;
		fstream.close();
		SaveState::MatchCurrentSave(*this);
	}
	void SaveState::Write() {
		std::ofstream fstream(file);
		fstream << std::setw(4) << this->raw;
		fstream.close();
	}

	SaveState SaveState::Read(std::string path) {
		return SaveState(path);
	}

	void SaveState::Update(json& j) {
		this->raw = j;
		SaveState::MatchCurrentSave(*this);
		this->Write();
	}

	void SaveState::MatchCurrentSave(SaveState& state) {
		//loop through every element currently existing in the newest save version and compare to the file to upgrade/add
		state.raw["major"] = CURRENT_MAJOR;
		state.raw["minor"] = CURRENT_MINOR;
		state.major = CURRENT_MAJOR;
		state.minor = CURRENT_MINOR;

		json updatedJson = json::parse(std::string( {
#include "../save.json"
		} ));

		bool changed = false;

		RecursiveJSONTraverse(updatedJson, state, changed, [](json::const_iterator it, json& updatedJson, SaveState& file, bool& changed, std::string absolutePath) { //lambda
			try {
				file.raw.at(json::json_pointer(absolutePath + it.key()));
			}
			catch (detail::exception& e) {
				if (e.id == 403) {
					file.raw[json::json_pointer(absolutePath + it.key())] = it.value();
					changed = true;
				}
			}
		});
		if(changed)
			state.Write();
	}
	template<class Func> void RecursiveJSONTraverse(json& j, SaveState& save, bool& changed, Func f, std::string absolutePath) {
		for (auto it = j.begin(); it != j.end(); ++it) {
			if (it->/*is_structured()*/is_object())
				RecursiveJSONTraverse(*it, save, changed, f, absolutePath + it.key() + "/"); //probe deeper
			else
				f(it, j, save, changed, absolutePath);
		}
	}
}