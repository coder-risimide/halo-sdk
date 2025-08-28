"""
@file    generate_clover_coords.py
@brief   Loads an image of a clover/flower, extracts its contour points,
         normalizes, scales, and translates them to fit a robot workspace,
         then saves as a C array for robotic arm use.

@author  Adithya
@date    2025-08-28

@details
- Loads an image using OpenCV
- Converts to grayscale and detects edges
- Finds contours and subsamples points
- Normalizes coordinates to center at origin
- Flips Y axis to convert image coordinates to math coordinates
- Scales to target radius and applies translation
- Saves coordinates as a C array
- Plots the final transformed points for visualization
"""

import cv2
import numpy as np
import matplotlib.pyplot as plt

# --- Parameters you can adjust ---
target_radius = 8.0   # Final max radius in cm (scales image size)
shift_x = -8.0         # Shift along X (cm)
shift_y = -1.0         # Shift along Y (cm)

# --- Load and process image ---
img = cv2.imread("flower.png")
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
edges = cv2.Canny(gray, 50, 150)
contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

coords = []
for contour in contours:
    for (x, y) in contour.reshape(-1, 2):
        coords.append((x, y))
coords = np.array(coords)

# Subsample (reduce number of points)
step = 5
coords = coords[::step]

# --- Normalize & scale to robot workspace ---
# 1. Center at origin
coords = coords - np.mean(coords, axis=0)

# 2. Flip Y (image coords -> math coords)
coords[:,1] = -coords[:,1]

# 3. Scale to fit inside target radius
max_extent = np.max(np.linalg.norm(coords, axis=1))
scale = target_radius / max_extent
coords = coords * scale

# 4. Translate flower
coords[:,0] += shift_x
coords[:,1] += shift_y

# --- Save as C array ---
with open("flower_coords.c", "w") as f:
    f.write("float flowerer_coords[][2] = {\n")
    for (x, y) in coords:
        f.write(f"    {{ {x:.2f}, {y:.2f} }},\n")
    f.write("};\n")

print(f"Saved {len(coords)} coordinates scaled & transformed for robot in flower_coords.c")

# --- Plot final transformed flower ---
plt.figure(figsize=(6,6))
plt.plot(coords[:,0], coords[:,1], 'k.', markersize=2)
circle = plt.Circle((0,0), 20, color='r', fill=False, linestyle="--")  # for reference
plt.gca().add_patch(circle)
plt.axis("equal")
plt.xlabel("X (cm)")
plt.ylabel("Y (cm)")
plt.title("Plot from coords")
plt.show()

