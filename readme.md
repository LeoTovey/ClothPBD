

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
122

# implicit cloth solver

根据隐式积分
$$
\mathbf{M} \frac{\mathbf{x}^{n+1}-\mathbf{x}^n-\Delta t \mathbf{v}^n}{\Delta t^2} = \mathbf{f}\left(\mathbf{x}^{n+1}\right)
$$
然后令
$$
\mathbf{h}\left(\mathbf{x}\right)= \frac{\mathbf{M}}{\Delta t^2} (\mathbf{x}-\mathbf{x}^n-\Delta t \mathbf{v}^n) -\mathbf{f}(\mathbf{x})
$$
这样问题就转化为了非线性方程求解的问题，我们可以利用牛顿法来进行迭代求解，把$\mathbf{h}(\mathbf{x})$利用泰勒公式在点$\mathbf{x}^{(k)}$展开


$$
\mathbf{h}\left(\mathbf{x}\right) \approx \mathbf{h}\left(\mathbf{x}^{(k)}\right) + \mathbf{h}'\left(\mathbf{x}^{(k)}\right)(\mathbf{x} - \mathbf{x}^{(k)}) \\
$$
因此迭代公式是
$$
\mathbf{x}^{(k+1)} = \mathbf{x}^{(k)} + \mathbf{h}'\left(\mathbf{x}^{(k)}\right)^{-1}\mathbf{h}\left(\mathbf{x}^{(k)}\right)
$$
然后我们对$\mathbf{h}(\mathbf{x})$进行求导，令弹力$\mathbf{f}(\mathbf{x})$对$\mathbf{x}$的导数为$\mathbf{J}(\mathbf{x}) = \frac{\partial \mathbf{f}(\mathbf{x})}{ \partial \mathbf{x}}$
$$
\mathbf{h}'\left(\mathbf{x}\right)= \frac{\mathbf{M}}{\Delta t^2} -\mathbf{J}(\mathbf{x})
$$
如何计算力的Jacobian矩阵：

对于一个只有两个质点的单个弹簧来说

<img src="../../Nutstore%20Files/%E6%88%91%E7%9A%84%E5%9D%9A%E6%9E%9C%E4%BA%91/Leo/assets/image-20230907153448485.png" alt="image-20230907153448485" style="zoom:50%;" />



弹簧的弹力是
$$
\mathbf{f}(\mathbf{x}_i) = \sum_{e = \{i, j\}}
- k \frac{\mathbf{x}_i - \mathbf{x}_j}{\| \mathbf{x}_i - \mathbf{x}_j \|} ( \| \mathbf{x}_i - \mathbf{x}_j \| - L_{ij})
$$
弹力求导为
$$
\frac{\partial \mathbf{f}(\mathbf{x}_i)}{ \partial \mathbf{x}_i} = \sum_{e = \{i, j\}} \frac{kL_{ij}}{\| \mathbf{x}_i - \mathbf{x}_j \|} - k -kL_{ij}\frac{(\mathbf{x}_i - \mathbf{x}_j) \cdot (\mathbf{x}_i - \mathbf{x}_j)^\top}{\| \mathbf{x}_i - \mathbf{x}_j \|^3}
$$
有些论文里面写成这样
$$
\frac{\partial \mathbf{f}(\mathbf{x}_i)}{ \partial \mathbf{x}_i} = \sum_{e = \{i, j\}} 
-k\frac{(\mathbf{x}_i - \mathbf{x}_j) \cdot (\mathbf{x}_i - \mathbf{x}_j)^\top}{\| \mathbf{x}_i - \mathbf{x}_j \|^2} - k(1 - \frac{L_{ij}}{\| \mathbf{x}_i - \mathbf{x}_j \|})(\mathbf{I} - \frac{(\mathbf{x}_i - \mathbf{x}_j) \cdot (\mathbf{x}_i - \mathbf{x}_j)^\top}{\| \mathbf{x}_i - \mathbf{x}_j \|^2})
$$
令
$$
\mathbf{J}_e =
-k\frac{(\mathbf{x}_i - \mathbf{x}_j) \cdot (\mathbf{x}_i - \mathbf{x}_j)^\top}{\| \mathbf{x}_i - \mathbf{x}_j \|^2} - k(1 - \frac{L_{ij}}{\| \mathbf{x}_i - \mathbf{x}_j \|})(\mathbf{I} - \frac{(\mathbf{x}_i - \mathbf{x}_j) \cdot (\mathbf{x}_i - \mathbf{x}_j)^\top}{\| \mathbf{x}_i - \mathbf{x}_j \|^2})
$$
对于上面两个弹力来说

作用在两个点上的弹簧力是
$$
\begin{aligned}

\mathbf{f}_0 &= 
- k \frac{\mathbf{x}_0 - \mathbf{x}_1}{\| \mathbf{x}_0 - \mathbf{x}_1 \|} ( \| \mathbf{x}_0 - \mathbf{x}_1 \| - L_{01})\\
\mathbf{f}_1 &= 
- k \frac{\mathbf{x}_1 - \mathbf{x}_0}{\| \mathbf{x}_1 - \mathbf{x}_0 \|} ( \| \mathbf{x}_1 - \mathbf{x}_0 \| - L_{01})\\
\end{aligned}
$$
因此
$$
\begin{aligned}
\mathbf{J}(\mathbf{x}) &= \frac{\partial \mathbf{f}(\mathbf{x})}{ \partial \mathbf{x}} \\
&= 
\begin{bmatrix}
 \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_0} & \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_1} \\
 \frac{\partial \mathbf{f}_1}{ \partial \mathbf{x}_0}  & \frac{\partial \mathbf{f}_1}{ \partial \mathbf{x}_1} 
\end{bmatrix} \\
&=
\begin{bmatrix}
 \mathbf{J}_e& - \mathbf{J}_e \\
 -\mathbf{J}_e & \mathbf{J}_e \\
\end{bmatrix}
\end{aligned}
$$
这是最简单的情况，现在我们把情况变复杂一些，使用两个弹簧

<img src="../../Nutstore%20Files/%E6%88%91%E7%9A%84%E5%9D%9A%E6%9E%9C%E4%BA%91/Leo/assets/image-20230907163307377.png" alt="image-20230907163307377" style="zoom:50%;" />

对于这种情况，我们来对弹簧的力进行求导，令$\mathbf{x}_{ij} = \mathbf{x}_i - \mathbf{x}_j$
$$
\begin{aligned}
\mathbf{f}_{e_{01}} &= 
- k \frac{\mathbf{x}_{01}}{\| \mathbf{x}_{01} \|} ( \| \mathbf{x}_{01} \| - L_{01})\\
\mathbf{f}_{e_{01}} &= 
- k \frac{\mathbf{x}_{01}}{\| \mathbf{x}_{01} \|} ( \| \mathbf{x}_{01} \| - L_{01})\\
\end{aligned}
$$

$$
\begin{aligned}
\mathbf{f}_0 &= 
- k \frac{\mathbf{x}_{01}}{\| \mathbf{x}_{01} \|} ( \| \mathbf{x}_{01} \| - L_{01})\\
\mathbf{f}_1 &= 
- k \frac{\mathbf{x}_{10}}{\| \mathbf{x}_{10} \|} ( \| \mathbf{x}_{10} \| - L_{01}) - k \frac{\mathbf{x}_{12}}{\| \mathbf{x}_{12} \|} ( \| \mathbf{x}_{12} \| - L_{12}) \\
\mathbf{f}_2 &= 
- k \frac{\mathbf{x}_{21}}{\| \mathbf{x}_{21} \|} ( \| \mathbf{x}_{21} \| - L_{12})\\
\end{aligned}
$$
上面的式子可以写成简单的形式
$$
\mathbf{f}(\mathbf{x}) = 
\begin{pmatrix} 
\mathbf{f}_0 \\ 
\mathbf{f}_1 \\
\mathbf{f}_2 \\
\end{pmatrix}
= 
\begin{pmatrix} 
\mathbf{f}_0 \\ 
-\mathbf{f}_0 -\mathbf{f}_2\\
\mathbf{f}_2 \\
\end{pmatrix}
$$
然后我们对$\mathbf{f}$求导
$$
\begin{aligned}
\mathbf{J}(\mathbf{x}) &= \frac{\partial \mathbf{f}(\mathbf{x})}{ \partial \mathbf{x}} \\
&= 
\begin{bmatrix}
 \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_0} & \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_1} & \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_2} \\
 \frac{\partial \mathbf{f}_1}{ \partial \mathbf{x}_0}  & \frac{\partial \mathbf{f}_1}{ \partial \mathbf{x}_1} & \frac{\partial \mathbf{f}_1}{ \partial \mathbf{x}_1} \\
 \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_0} &  \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_1}  & \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_2}
\end{bmatrix} \\
&=
\begin{bmatrix}
 \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_0} & \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_1} & \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_2} \\
 
  - \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_0} - \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_0} 
 &   - \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_1} - \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_1} 
 &   - \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_2} - \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_2} 
 \\
 \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_0} &  \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_1}  & \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_2}
\end{bmatrix} \\

&= 
\begin{bmatrix}
 \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_0} & \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_1} & \mathbf{0} \\
 
  - \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_0} 
 &   - \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_1} - \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_1} 
 &    - \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_2} 
 \\
\mathbf{0} &  \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_1}  & \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_2}
\end{bmatrix} \\
&= 
\begin{bmatrix}
 \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_0} & \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_1} & \mathbf{0} \\
 -\frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_0} & - \frac{\partial \mathbf{f}_0}{ \partial \mathbf{x}_1}& \mathbf{0}\\
\mathbf{0} & \mathbf{0} & \mathbf{0} \\
\end{bmatrix}  +
\begin{bmatrix}
\mathbf{0} & \mathbf{0} & \mathbf{0} \\ 
\mathbf{0} & -\frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_1} & - \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_2} \\
\mathbf{0} &  \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_1}  & \frac{\partial \mathbf{f}_2}{ \partial \mathbf{x}_2}
\end{bmatrix}  \\

&=
\begin{bmatrix}
\mathbf{J}_{e_{01}}&  -\mathbf{J}_{e_{01}}& \mathbf{0} \\ 
 -\mathbf{J}_{e_{01}} & \mathbf{J}_{e_{01}} & \mathbf{0} \\ 
\mathbf{0} & \mathbf{0} & \mathbf{0} \\ 
\end{bmatrix}  
+
\begin{bmatrix}
\mathbf{0} & \mathbf{0} & \mathbf{0} \\
\mathbf{0} & \mathbf{J}_{e_{12}} &  - \mathbf{J}_{e_{12}} \\ 
\mathbf{0} & - \mathbf{J}_{e_{12}} & \mathbf{J}_{e_{12}} \\ 
\end{bmatrix} 
\end{aligned}
$$
可以看到可以分解为多个子弹簧的jacobian矩阵之和

因此推广到任意情况下的jacobian矩阵
$$
\begin{aligned}
\mathbf{J}(\mathbf{x}) &= \frac{\partial \mathbf{f}(\mathbf{x})}{ \partial \mathbf{x}} \\
&= 
\sum_{e = \{i, j\}}
\begin{bmatrix}
\mathbf{0} & \cdots& \cdots& \cdots & \mathbf{0} \\
\vdots & \mathbf{J}_e & \cdots & -\mathbf{J}_e & \vdots \\
\vdots & \vdots & \ddots & \vdots & \vdots \\
\vdots & -\mathbf{J}_e & \cdots &\mathbf{J}_e & \vdots \\
\mathbf{0} & \cdots & \cdots& \cdots& \mathbf{0} \\
\end{bmatrix}
\\
\end{aligned}
$$


然后我们可以把问题转化为求解线性方程组
$$
\begin{aligned}
\mathbf{h}'\left(\mathbf{x}^{(k)}\right)\Delta\mathbf{x} &= -\mathbf{h}\left(\mathbf{x}^{(k)}\right) \\
(\frac{1}{\Delta t^2}\mathbf{M} -\mathbf{J}(\mathbf{x}^{(k)}))\Delta\mathbf{x} &=  -\frac{1}{\Delta t^2}\mathbf{M}(\mathbf{x}^{(k)}-\mathbf{x}^n-\Delta t \mathbf{v}^n) +\mathbf{f}(\mathbf{x}^{(k)}) \\
\end{aligned}
$$
如何解这个线性方程组呢？

- 使用某些矩阵近似Jacobian矩阵，例如使用常对角矩阵
- 使用迭代法来求解这个线性方程组
- 



所以：


$$
\begin{aligned}
\mathbf{f}\left(\mathbf{x}\right) &= \frac{\mathbf{M}}{\Delta t^2} (\mathbf{x}-\mathbf{x}^n_i-\Delta t \mathbf{v}^n_i) -\mathbf{f}_i(\mathbf{x}) \\
\mathbf{f}_i(\mathbf{x}) &= \mathbf{Mg} + \sum_j
- k \frac{\mathbf{x} - \mathbf{x}^{n+1}_j}{\| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \|} ( \| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \| - L_{ij})
\end{aligned}
$$


布



$$
\begin{bmatrix}
  A & \mathbf{0} \\
  \mathbf{0} & B
\end{bmatrix}
\begin{bmatrix}
  0 & 0 & 0 & \cdots & 0 \\
  0 & 0 & 0 & \cdots & 0 \\
  \vdots & \vdots & \vdots & \ddots & \vdots \\
  0 & 0 & 0 & \cdots & 0
\end{bmatrix}
$$

$$
\begin{aligned}
\mathbf{M} \frac{\mathbf{x}^{n+1}-\mathbf{x}^n-\Delta t \mathbf{v}^n}{\Delta t^2} &=\mathbf{Mg} + f_{spring}\left(\mathbf{x}^{n+1}\right) \\
f_{spring}\left(\mathbf{x}^{n+1}_i\right) &= \sum_j
- k \frac{\mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j}{\| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \|} ( \| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \| - L_{ij})
\end{aligned}
$$

接下来转化为求根的问题

$$
\mathbf{F}\left(\mathbf{x}^{n+1}_i\right)= \frac{m}{\Delta t^2} (\mathbf{x}^{n+1}_i-\mathbf{x}^n_i-\Delta t \mathbf{v}_i^n) - m\mathbf{g} - f_{spring}(\mathbf{x}^{n+1}_i)
$$

$$
\begin{aligned}
h'\left(\mathbf{x}^{n+1}_i\right) &= \frac{m}{\Delta t^2} - f_{spring}'(\mathbf{x}^{n+1}_i) \\
f_{spring}'\left(\mathbf{x}^{n+1}_i\right) & \approx \sum_j
- k  \approx -4k\\
\end{aligned}
$$


这里之前games103的做法是简化了弹簧弹力的求导，这里我们可以进一步进行计算一下
$$
\begin{aligned}
f_{spring}\left(\mathbf{x}^{n+1}_i\right) &= \sum_j
- k \frac{\mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j}{\| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \|} ( \| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \| - L_{ij}) \\
f_{spring}’\left(\mathbf{x}^{n+1}_i\right) &= \frac{L_{ij}k}{{\| \mathbf{x}^{n+1}_i - \mathbf{x}^{n+1}_j \|}} \mathbf{I} 
- k \mathbf{I}
\\
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





参考：

https://zhuanlan.zhihu.com/p/444105016

