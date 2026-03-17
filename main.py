import matplotlib.pyplot as plt
import re

feature_sets = []
accuracies = []
feature_sizes = []

with open("results.txt") as f:
    for line in f:
        match = re.search(r'Feature set \{([^}]*)\} was best, accuracy is ([\d.]+)%', line)
        
        if match:
            features = match.group(1)
            accuracy = float(match.group(2))

            # store full feature set string
            feature_sets.append("{" + features + "}")
            accuracies.append(accuracy)

            # count number of features
            if features.strip() == "":
                size = 0
            else:
                size = len(features.split(','))
            feature_sizes.append(size)

# ---- GRAPH ----
plt.figure()

plt.plot(feature_sizes, accuracies, marker='o')

plt.xlabel("Number of Features")
plt.ylabel("Accuracy (%)")
plt.title("Backward Elimination (Best Feature Sets)")

plt.grid()

# annotate points with feature sets
for i in range(len(feature_sets)):
    plt.text(feature_sizes[i], accuracies[i], feature_sets[i], fontsize=7, rotation=30)

plt.tight_layout()
plt.show()