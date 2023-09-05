

# Template
<https://leotovey.github.io/ClothPBD/>
## Install
This project uses [node](http://nodejs.org) and [npm](https://npmjs.com). Go check them out if you don't have them locally installed.

## Usage


``` bash
# Install dependencies (only for first time)
npm i

# Run the local dev server
npm run dev

# Build for production in the dist/ directory
npm run build
```

# mass-springs system

# implicit cloth solver
$$
\begin{aligned}
\mathbf{M} \frac{\mathbf{x}^{n+1}-\mathbf{x}^n-\Delta t \mathbf{v}^n}{\Delta t^2} &=\mathbf{Mg} + f_{spring}\left(\mathbf{x}^{n+1}\right) \\
f_{spring}\left(\mathbf{x}^{n+1}_i\right) &= \sum_j
- k \frac{\mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j}{\| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \|} ( \| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \| - L_{ij})
\end{aligned}
$$

接下来转化为求根的问题

$$
h\left(\mathbf{x}^{n+1}_i\right)= \frac{m}{\Delta t^2} (\mathbf{x}^{n+1}_i-\mathbf{x}^n_i-\Delta t \mathbf{v}_i^n) - m\mathbf{g} - f_{spring}(\mathbf{x}^{n+1}_i)
$$

$$
\begin{aligned}
h'\left(\mathbf{x}^{n+1}_i\right) &= \frac{m}{\Delta t^2} - f_{spring}'(\mathbf{x}^{n+1}_i) \\
f_{spring}'\left(\mathbf{x}^{n+1}_i\right) & \approx \sum_j
- k  \approx -4k\\
\end{aligned}
$$
使用牛顿法求解
$$
\begin{aligned}
\mathbf{x}^{i+1}  &= \mathbf{x}^{i} - \frac{h\left(\mathbf{x}_i\right)}{h'\left(\mathbf{x}_i\right)} \\
\mathbf{x}^{i+1}  &= \mathbf{x}^{i} - (\frac{m}{\Delta t^2} +4k)^{-1}h\left(\mathbf{x}_i\right)
\end{aligned}
$$

因此最后的流程为
- 输入为$\mathbf{x}^n$
- 令 $\mathbf{x}^0 = \mathbf{x}^n$
- 令 $\hat{\mathbf{x}} = \mathbf{x}+\Delta t \mathbf{v}$
- 迭代 1...k 次
	- 计算$h\left(\mathbf{x}^{k}_i\right) = \frac{m}{\Delta t^2} (\mathbf{x}^{k}_i-\hat{\mathbf{x}_i}) - m\mathbf{g} $
	- 然后遍历每一根弹簧，对于顶点i，j
		- $h\left(\mathbf{x}^{k}_i\right) \leftarrow h\left(\mathbf{x}^{k}_i\right) + k \frac{\mathbf{x}^{k}_i - \mathbf{x}^{k}_j}{\| \mathbf{x}^{k}_i - \mathbf{x}^{k}_j \|} ( \| \mathbf{x}^{k}_i - \mathbf{x}^{k}_j \| - L_{ij})$
		- $h\left(\mathbf{x}^{k}_j\right) \leftarrow h\left(\mathbf{x}^{k}_j\right) - k \frac{\mathbf{x}^{k}_i - \mathbf{x}^{k}_j}{\| \mathbf{x}^{k}_i - \mathbf{x}^{k}_j \|} ( \| \mathbf{x}^{k}_i - \mathbf{x}^{k}_j \| - L_{ij})$
	- 最后得到$\mathbf{x}^{k+1}_i  = \mathbf{x}^{k}_i - (\frac{m}{\Delta t^2} +4k)^{-1}h\left(\mathbf{x}^k_i\right)$
- 输出为$\mathbf{x}^{n+1}$

