-- Song file structure
-- This is just an outline of it and this will change.

-- API specification:
--   note_on(note, sf_preset, effect)
--   note_off(note)
--   delay(millisecons)
--   play_pattern(pattern_name, repeat)

-- Effects:
--   delay
--   reverb

-- This is a pattern definition.
-- Columns go from A..F and rows go from 1..4.
-- This blocks will be visualized in the DAW.
pattern("A1", function(self)
	for i = 1, 10 do
		note_on(40 + i, 1, nil)
		delay(10)
	end
end)

-- This is the actual song timeline.
timeline(function(self)
	play_pattern("A1", 1)
	play_pattern("F2", 3)
	play_pattern("D4", 2)
end)

