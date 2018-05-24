// apres_MIDI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Decoder.h"
#include "HeaderChunk.h"
#include "TrackChunk.h"
#include <memory>

#include <iomanip>

int main()
{
	auto d = Decoder();
	d.SetMIDIBuffer("C:\\Users\\david\\Downloads\\Fagning.mid");

	MIDI a = d.GetMIDI();

	for (int i = 0; i < a.chunks.size(); ++i)
	{
		MIDIChunk*& c = a.chunks[i];

		std::cout << "Chunk #" << i << ": " << std::endl;
		std::cout << "Type: " << (c->type == HEADER ? "Header" : "Track") << std::endl << std::endl;
		std::cout << std::setfill('-') << std::setw(100) << "" << std::endl;
		switch (c->type)
		{
			case HEADER:
				std::cout << dynamic_cast<HeaderChunk*>(c)->Info();
				break;
			case TRACK:
				std::cout << dynamic_cast<TrackChunk*>(c)->Info();
				break;
		}
		std::cout << std::setfill('-') << std::setw(100) << "" << std::endl;

		std::cout << std::endl << std::endl << std::endl;
	}
	system("pause");
	
    return 0;
}

