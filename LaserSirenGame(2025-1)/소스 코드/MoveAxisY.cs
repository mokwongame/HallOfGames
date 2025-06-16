using UnityEngine;

public class MoveAxisY : MonoBehaviour
{
    [SerializeField] float speed = 10.0f;
    [SerializeField] float ymin = -10.0f;
    [SerializeField] float ymax = 10.0f;


    void Update()
    {
        //Input.GetAxis("Mouse Y")��ü�� �� �����ӿ� �̵��� �Ÿ��̱� ������ Time.deltaTime�� ������� ����
        float yoff = Input.GetAxis("Mouse Y") * speed;
        float ypos = transform.localPosition.y + yoff;

        if (ypos < ymin) yoff += ymin - ypos;
        else if (ypos > ymax) yoff += ymax - ypos;

        transform.Translate(0, yoff, 0);

    }
}
