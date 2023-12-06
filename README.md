# Monte-Carlo-Integration

##### 蒙特卡洛积分

<br>

$\ \hat{I} = \frac{1}{N}\sum_{i=0}^{N}\frac{f(x_i)}{p(x_i)} $


<br>

##### 渲染方程
<br>


$ L_{o}(p, w_{o}) = L_e(p, w_{o}) + \int_{\Omega}f_{r}(p, w_i, w_o)L_i(p, w_i)(n·w_i)dw_i $


<br>

##### 绝对漫反射材质的Cosine Weight

<br>

不考虑自发光的前提下


$ f_{r}(p, w_i, w_o)  $ 是均匀的，即 $ \frac{p}{\pi} $, $ p $ 为 albedo

$ L_{o}(p, w_{o}) = \frac{L_{i}p}{\pi}\int_{\Omega}{ (n·w_i)}dw_i $

<br>

$ L_{o}(p, w_{o}) = \frac{L_{i}p}{\pi}\int_{0}^{2\pi}\int_{0}^{\frac{\pi}{2}}sin(\theta){cos(\theta)}dw_i $ 

<br>
 
求出 $ pdf(\omega) = \frac{cos(\theta)}{\pi}$



<br>

##### GGX Weight
