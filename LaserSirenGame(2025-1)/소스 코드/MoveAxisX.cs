using UnityEngine;

public class MoveAxisX : MonoBehaviour
{
    [SerializeField] float speed = 10.0f;
    [SerializeField] float xmin = -10.0f;
    [SerializeField] float xmax = 10.0f;

    
    void Update()
    {
        //Input.GetAxis("Mouse X")��ü�� �� �����ӿ� �̵��� �Ÿ��̱� ������ Time.deltaTime�� ������� ����
        float xoff = Input.GetAxis("Mouse X") * speed;
        float xpos = transform.localPosition.x + xoff;

        if (xpos < xmin) xoff += xmin - xpos;
        else if (xpos > xmax) xoff += xmax - xpos;

        transform.Translate(xoff, 0, 0);
    }
}
