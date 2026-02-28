## Malloq, The Mighty

*A learning project exploring navigation algorithms and logic for an autonomous mowing robot.*

Malloq is an exploratory project pushing the boundaries of robot autonomy. The core challenge is to navigate a lawn-mowing robot entirely without external positioning data (no GPS, no RTK). All intelligence relies on internal sensing and relative dead reckoning.

### Core Focus & Challenges
* **Odometry & IMU Fusion:**  Combining data from wheel encoders and inertial sensors to estimate pose and heading.
* **Mitigating Error Drift:** Developing strategies to correct accumulated positioning errors that occur when navigating without a fixed external reference.
* **Logical Coverage:**       Designing algorithms to ensure efficient area coverage without the aid of a global map.
