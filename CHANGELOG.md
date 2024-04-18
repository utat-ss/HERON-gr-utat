# v2.1.1

## Fixes

- Block `Tagged Stream Fixed Length Padder`
    - Moved logging to debug line since it was cluttering up terminal output.

# v2.1.0

## Highlights

- Introduced new blocks:
    - `ESTTC Header Format`
    - `ESTTC Framer`
    - `ESTTC Deframer`
    - `Tagged Stream Fixed Length Padder`
    - `Variable Filter`
- Marked `HERON RX BB` block as deprecated

## Features

- `ESTTC Header Format` is the first step in a new method of deframing ESTTC frames. The hope is to utilize GNURadio's inbuilt digital modules to do most of the dirty work. This will make the code much more readable and maintainable in addition to being smaller.
- `ESTTC Framer` and `ESTTC Deframer` are what utilize the `ESTTC Header Format` object to wrap/unwrap messages (specifically PDU's).
- `Tagged Stream Fixed Length Padder` is a hack to ensure packets are properly streamed out of an SDR.
- `Variable Filter` is NOT a signal filter, but rather a message filter (the messages represent variables to be changed).

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
