import cv2

operation = input("What would you like to test? (cam_ids/concurrent_cams)")

def test_cam_ids():
    for i in range(0, 9):
        try:
            print("-----------")
            print("Trying ", i)

            cv2.VideoCapture(i)
        except:
            pass

def concurrent_cams(n_cams, ids):
    if n_cams == 1:
        c1 = cv2.VideoCapture(ids[0])
        for i in range(0, 6):
            r1, f1 = c1.read()
            print("Stage", i)
    elif n_cams == 2:
        c1 = cv2.VideoCapture(ids[0])
        c2 = cv2.VideoCapture(ids[1])
        for i in range(0, 6):
            r1, f1 = c1.read()
            r2, f2 = c2.read()
            print("Stage", i)
    elif n_cams == 3:
        c1 = cv2.VideoCapture(ids[0])
        c2 = cv2.VideoCapture(ids[1])
        c3 = cv2.VideoCapture(ids[2])
        for i in range(0, 6):
            r1, f1 = c1.read()
            r2, f2 = c2.read()
            r3, f3 = c3.read()
            print("Stage", i)

    print("Complete!")

if operation == 'cam_ids':
    test_cam_ids()

elif operation == 'concurrent_cams':
    n_cams = int(input('How many cams?'))
    ids = []
    for i in range(0, n_cams):
        result = int(input("id %i " % i))
        ids.append(result)
    concurrent_cams(n_cams, ids)
