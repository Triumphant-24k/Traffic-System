import pandas as pd
from sklearn.tree import DecisionTreeClassifier, export_text

# Load dataset
df = pd.read_csv("traffic_history.csv")

# Features and label
X = df[["hour", "vehicle_count"]]
y = df["busy_label"]

# Train model
model = DecisionTreeClassifier(max_depth=3, random_state=42)
model.fit(X, y)

# Print learned rules
print("Learned Decision Tree Rules:\n")
print(export_text(model, feature_names=["hour", "vehicle_count"]))

# Predict for all 24 hours using the dataset values
predictions = model.predict(X)

df["predicted_busy"] = predictions

print("\nPredictions:")
print(df)

# Build busy hour array for ESP32
busy_hours = [False] * 24

for _, row in df.iterrows():
    if row["predicted_busy"] == 1:
        busy_hours[int(row["hour"])] = True

print("\nESP32 busy hour array:")
print(busy_hours)

# Also print in C-style format for easy copy-paste
cpp_array = ", ".join(["true" if x else "false" for x in busy_hours])
print("\nPaste into ESP32 code:")
print(f"bool predictedBusyHour[24] = {{ {cpp_array} }};")