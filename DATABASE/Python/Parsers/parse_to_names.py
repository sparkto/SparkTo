from urllib import request
from bs4 import BeautifulSoup

def get_man_names():
    try:
        url = 'http://kakzovut.ru/man.html'
        return_names = []
        myheader = {}
        myheader['User-Agent'] = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 ' \
                                 '(KHTML, like Gecko) Chrome/61.0.3163.79 Safari/537.36'

        req = request.Request(url, headers=myheader)
        answer = BeautifulSoup(request.urlopen(req), 'html5lib')
        names = str(answer.findAll(True, "nameslist")).split('<div ')
        names = [x for i, x in enumerate(names, 1) if i % 2 == 0]
        for i in range(len(names)):
            if names[i].find('(') == -1:
                return_names.append(names[i].split('>')[2].split('<')[0])

        return return_names
    except Exception as connection_error:
        print("Error", connection_error)
def main():
    file = open('man_names', 'w')
    names = get_man_names()
    file.write('{}\n'.format(str(len(names))))
    for i in range(len(names)):
        file.write('{}\n'.format(names[i]))
if __name__ == '__main__':
    main()

