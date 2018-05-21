#include "stdafx.h"
#include "TrackEvent.h"
#include "Helpers.h"

#include <algorithm>
#include <map>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <iostream>

#define stringtolower(x) std::transform(x.begin(), x.end(), x.begin(), ::tolower);


TrackEvent::TrackEvent()
{
}
	
void TrackEvent::SetEvent(std::vector<uint8_t>::iterator event, std::vector<uint8_t>::iterator chunk_boundary)
{
	std::tuple<Event, size_t> current_match = std::make_tuple(UNDEFINED, 0);

	for (auto arg : this->signatures)
	{
		if (arg.first == UNDEFINED) continue;

		if ((chunk_boundary - event) < (arg.second.size() + 1) / 3 )
		{
			continue;
		}
		std::tuple<bool, size_t> potential_match = this->WildcardMatchEvent(event, arg.second);

		// We give precedence to the longest signature, because those should be more precise
		if (std::get<0>(potential_match) && arg.second.size() > this->signatures[std::get<0>(current_match)].size())
		{
			current_match = std::make_tuple(arg.first, std::get<1>(potential_match));
		}
	}


	if (std::get<0>(current_match) == UNDEFINED)
	{
		Helpers::AssertError(std::get<0>(current_match) == UNDEFINED, "Undefined MIDI event encountered.");
	}

	this->type = std::get<0>(current_match);

	auto boundary = event + std::get<1>(current_match) - (this->signatures[std::get<0>(current_match)].size() + 1) / 3;

	this->signature = std::vector<uint8_t>(event, boundary);
	this->data = std::vector<uint8_t>(boundary, event + std::get<1>(current_match));
}

TrackEvent::~TrackEvent()
{
}





std::tuple<bool, size_t> TrackEvent::WildcardMatchEvent(std::vector<std::uint8_t>::iterator data, std::string pattern)
{
	size_t bytes_in_pattern = (pattern.size() + 1) / 3;

	
	std::string data_hex = Helpers::ToHex(std::vector<std::uint8_t>(data, data + bytes_in_pattern) );
	stringtolower(pattern);
	stringtolower(data_hex);

	uint8_t size = 0;

	for (size_t i = 0; i < pattern.size(); ++i)
	{
		if (
			(
				(pattern[i] == data_hex[i]) ||
				((pattern[i] == '+' || pattern[i] == 'l') && isxdigit(data_hex[i])) ||
				(pattern[i] == '-' && data_hex[i] >= '0' && data_hex[i] <= '7') ||
				(pattern[i] == '/' && data_hex[i] >= '0' && data_hex[i] <= '1')
			) == false
		)
		{
			
			return std::make_tuple(false, 0);
		}
		
		if (i != 0 && pattern[i] == 'l' && pattern[i - 1] == 'l') size = (uint8_t)strtol(data_hex.substr(i - 1, i + 1).c_str() , NULL, 16);
	}

	return std::make_tuple(true, (pattern.size() + 1) / 3 + size);	

}