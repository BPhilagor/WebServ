FROM archlinux

RUN pacman -Syu --noconfirm gcc git make zsh valgrind vim bat

RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" -- \
# RUN sh -c "$(wget -O- https://github.com/deluan/zsh-in-docker/releases/download/v1.1.5/zsh-in-docker.sh)" -- \
#     -t eastwood

# ENV DEBUG=1

ENV DEBUGINFOD_URLS="https://debuginfod.archlinux.org"
ENV WSL_DISTRO_NAME=docker

RUN rm ~/.zshrc
RUN echo 'export ZSH="$HOME/.oh-my-zsh"' > ~/.zshrc
RUN echo 'ZSH_THEME="eastwood"' >> ~/.zshrc
RUN echo 'plugins=(git)' >> ~/.zshrc
RUN echo 'source $ZSH/oh-my-zsh.sh' >> ~/.zshrc

RUN echo 'alias val="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt" ./' >> ~/.zshrc && \
	chmod +x ~/.zshrc

# COPY . cpp42_docker
# WORKDIR cpp42_docker
ENTRYPOINT [ "/bin/zsh" ]

# docker build -t webserv .
# docker run -itv $(pwd):/local webserv
