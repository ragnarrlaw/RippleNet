# RippleNet

![RippleNet Architecture](assets/logo.jpg)

**RippleNet** is a minimalist, research-oriented Deep Learning framework written in pure **C**. 

Designed to deconstruct high level frameworks. For educational purposes only. RippleNet is built from scratch—without external math libraries like BLAS or LAPACK—to explore the systems-level constraints of Deep Learning, from memory strides to distributed orchestration.

> "To understand the machine, you must build the machine."

## ⚡ Core Philosophy
* **Zero Magic:** No hidden computational graphs or auto-magical abstractions.
* **Pure C:** Manual memory management, custom allocators, and raw pointers.
* **Systems First:** Built with distributed systems, CRIU checkpointing, and GNNs in mind.

## 🛠 Features (v0.0.0)
- [x] **Tensor Engine:** N-Dimensional array support with strided views.
- [x] **Memory Abstraction:** Separation of `Storage` (heavy data) and `Matrix` (lightweight views).
- [ ] **Autograd:** Reverse-mode automatic differentiation (Coming Soon).
- [ ] **Accelerator:** CUDA kernels for GPU support.
- [ ] **Graph Neural Networks:** Sparse matrix support and message passing.
