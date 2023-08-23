# v2.1.0

## Highlights

- Introduced `ESTTC Header Format` block
- Introduced `ESTTC framer` block
- Marked `HERON RX BB` as deprecated

## Features

- `ESTTC Header Format` is the first step in a new method of deframing ESTTC frames. The hope is to utilize GNURadio's inbuilt digital modules to do most of the dirty work. This will make the code much more readable and maintainable in addition to being smaller.
- `ESTTC Framer` is what utilizes the `ESTTC Header Format` object to wrap messages (specifically PDU's) in an ESTTC packet.

# v2.0.0

## Highlights

- Compatible with GNURadio v3.10
- Codebase was initiated from scratch

## Features

- `HERON RX BB` block to decode packets coming from the satellite

## Potential Problems / Future Fixes

- No indication if CRC fails

# v1.0.0

- Original codebase was not passing tests and lots of bugs were identified
- Folder structure was outdated (compatible with GNURadio v3.9)
