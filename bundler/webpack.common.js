const CopyWebpackPlugin = require('copy-webpack-plugin')
const HtmlWebpackPlugin = require('html-webpack-plugin')
const MiniCSSExtractPlugin = require('mini-css-extract-plugin');
const path = require('path')
const webpack = require('webpack')
module.exports = {
    entry: path.resolve(__dirname, '../src/index.ts'),
    experiments: {
        asyncWebAssembly: true,
        syncWebAssembly: true
      },
    output:
    {
        hashFunction: 'xxhash64',
        filename: 'bundle.[contenthash].js',
        path: path.resolve(__dirname, '../dist')
    },
    devtool: 'source-map',
    plugins:
    [
        new CopyWebpackPlugin({ patterns : [{ from: path.resolve(__dirname, '../static') },], }),
        new HtmlWebpackPlugin({
            template: path.resolve(__dirname, '../src/index.html'),
            minify: true
        }),
        new MiniCSSExtractPlugin()
    ],
    resolve: {
        alias: {
            three: path.resolve('./node_modules/three'),
            module: require.resolve('module')
        },
        extensions: ['.tsx', '.ts', '.js'],
        fallback: {
            // 👇️👇️👇️ add this 👇️👇️👇️
            "fs": false,
            "os": false,
            "path": false,
            "util":false,
            "assert": false,
             "tty": false,
            "stream": false,
            "buffer": false,
          }
    },
    module: {
        rules: [
            // HTML
            {
                test: /\.(html)$/,
                use: ['html-loader'],
            },
            // {
            //     test: /\.(wasm)$/,
            //     use: ['wasm-loader'],
            // },
            // JS
            {
                test: /\.tsx$/,
                exclude: /node_modules/,
                use: ['babel-loader'],
            },

            // CSS
            {
                test: /\.css$/,
                use: [MiniCSSExtractPlugin.loader, 'css-loader'],
            },

            // Images
            {
                test: /\.(jpg|png|gif|svg)$/,
                type: 'asset/resource',
                generator: {
                    filename: 'assets/images/[hash][ext]',
                },
            },
            // Audio
            {
                test: /\.(mp3|wav)$/,
                loader: 'file-loader',
                options: {
                    name: '[path][name].[ext]',
                },
            },
            // Fonts
            {
                test: /\.(ttf|eot|woff|woff2)$/,
                type: 'asset/resource',
                generator: {
                    filename: 'assets/fonts/[hash][ext]',
                },
            },
            // Shaders
            {
                test: /\.(glsl|vs|fs|vert|frag)$/,
                exclude: /node_modules/,
                use: ['glslify-import-loader', 'raw-loader', 'glslify-loader'],
            },
        ],
    },
}
