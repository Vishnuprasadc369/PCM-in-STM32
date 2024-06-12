import numpy as np

# Replace with the path to your raw file
filename = 'C:/Users/Hp/Documents/Audacity/hello.raw'

# Read the raw audio file
audio_data = np.fromfile(filename, dtype=np.uint8)

# Print the audio data array
print(audio_data)

# Save audio data to a file with comma-separated values and without newlines
with open('audio_data.txt', 'w') as f:
    audio_data_str = ','.join(map(str, audio_data))
    f.write(audio_data_str)

print("DONE!")