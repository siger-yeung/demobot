# demobot

A demobot written in C++ with Metamod.

## Build

The building system of this project is powered by [xmake](https://xmake.io/).

```bash
xmake f -a i386 -m debug --ccache=n
xmake
```

For Linux HLDS, it is strongly encouraged to build the mod with GCC 7.5.0. For convenience,
we have packed GCC 7.5.0 and xmake into a docker.

```
export UID=$(id -u)
export GID=$(id -g)
docker-compose up demobot-compile
```

Then you can enter the environment by:

```
docker-compose exec demobot-compile bash
```

## TO-DO

* [ ] Support both "gamedll" and metamod loading.
* [ ] Utilize more types of HLDEMO frames (currently only frame type 2).
* [ ] Make the plugin configurable.
